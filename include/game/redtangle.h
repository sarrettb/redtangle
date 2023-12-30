#pragma once
#include <iostream> 
#include <vector> 
#include <memory> 
#include <string> 
#include <any> 

// Declares all data structres, classes, and methods to play a redtangle game
// Independent of any libraries. i.e. SDL, gRPC
namespace redtangle {
    // Commonly used data types 
    struct Point {
        int x, y;
    };
    struct Rect { // defines a rectangle for rendering 
       Point top_left;
       int w, h;
       friend std::ostream & operator << (std::ostream &out, const Rect& r) { 
        std::cout << "{ " << r.top_left.x << ", " << r.top_left.y << "," << r.w << "," << r.h << "}" << std::endl; 
        return out;
      }
    };
    struct Circle { // defines a circle for rendering
        Point center;
        int r;
    };
    struct Color { // defines a color for rendering 
        int red, green, blue; 
        bool operator == (const Color& c) const {return this->red == c.red && this->blue == c.blue && this->green == c.green; }
        bool operator != (const Color& c) const {return this->red != c.red && this->blue != c.blue && this->green != c.green; } 
    };
    struct Location { // location relative to the actual game (8x8 grid)
        int x, y;
        Location(int xx, int yy) : x(xx), y(yy) {};
        bool operator == (const Location& loc) const {return this->x == loc.x && this->y == loc.y; }
        bool operator != (const Location& loc) const {return this->x != loc.x && this->y != loc.y; }
        friend std::ostream & operator << (std::ostream &out, const Location& loc) { std::cout << "{ " << loc.x << ", " << loc.y << "}" << std::endl; return out;}
    };
    enum class Side { // values map to an index of the sides of a piece
        LEFT = 0,
        TOP = 1,
        RIGHT = 2,
        BOTTOM = 3
    };

    // Commonly used constants 
    static const int GRID_SIZE = 8; 
    static const Location INVALID = {-1, -1}; 
    static const Color BLACK = { 30, 28, 15 }; 
    static const Color WHITE = { 204, 203, 202 }; 
    static const Color RED = {200, 31, 32};
    static const Color GOLD = {216, 188, 35}; 
    static const Color NONE = {-1, -1, -1 }; 

    // Helper function 
    inline bool in_redtangle(const Location& loc) { return (loc.y == 0 || loc.y == 1 || loc.y == 6 || loc.y == 7) && (loc.x >= 2 && loc.x < 6); }

    
    // Interface for rendering the redtangle game
    // One implementation will be to run the game headless (no UI) which will be used by the gRPC Server 
    // Another implementation will use SDL2 to render the UI 
    // Any other rendering libraries could be used by making a class that inherits from Redtangle_UI
    // Additionally, this lets the actual game logic not be dependent on rendering libraries 
    class RedtangleUI {
        protected:
            Rect _redtangle_surface; 
            int _window_width;
            int _window_height; 
            double _rect_width;
            double _rect_height; 
            int _edge_size; 
            double _piece_width;
            double _piece_height;
            int _inner_radius;
            int _outer_radius;
            bool _debugging; 

            // Set the window surface -> the area that the actual game is on (not the status or menu bar)
            // Coerce to integer multiple of 8
            void coerce_redtangleSurface() { 
                _redtangle_surface.w = _redtangle_surface.w - (_redtangle_surface.w % GRID_SIZE); 
                _redtangle_surface.h = _redtangle_surface.h - (_redtangle_surface.h % GRID_SIZE); 
                update_params(); 
            }
            // Update rendering parameters dependent on size of surface
            void update_params() {  
                _rect_width = _redtangle_surface.w / GRID_SIZE;
                _rect_height = _redtangle_surface.h / GRID_SIZE;
                _edge_size = 2;
                _piece_width = 0.72 * (_rect_width - _edge_size);
                _piece_height = 0.72 * (_rect_height - _edge_size);
                _inner_radius = 0.14 * _piece_width;
                _outer_radius = 0.18 * _piece_width; 
            }
        public: 
            enum class EventType {
                QUIT = 0,
                SELECTION = 1,
                END_TURN = 2,
                ROTATION_CLOCKWISE = 3,
                ROTATION_COUNTERCLOCKWISE = 4,
                UI_EVENT = 5,
                UNKNOWN = -1
            }; 
            RedtangleUI(int width, int height) : _window_width(width), _window_height(height), _redtangle_surface({{0, 0}, width, height}) { coerce_redtangleSurface(); }
            virtual Rect get_redtangleSurface() const {return _redtangle_surface; }
            virtual int get_windowWidth() const {return _window_width;}
            virtual int get_windowHeight() const {return _window_height;}
            virtual int get_rectWidth() const { return _rect_width; }
            virtual int get_rectHeight() const { return _rect_height; }
            virtual int get_pieceWidth() const { return _piece_width; }
            virtual int get_pieceHeight() const { return _piece_height; } 
            virtual int get_innerRadius() const { return _inner_radius; }
            virtual int get_outerRadius()  const { return _outer_radius; }
            virtual int get_edgeSize() const { return _edge_size; } 
            virtual bool get_debugFlag() const { return _debugging; }
            virtual Location get_location() const = 0;  
            virtual EventType get_eventType() = 0; 
            virtual void render_filledRect(const Rect& rect, const Color& color) const = 0; 
            virtual void render_filledCircle(const Circle& circle, const Color& color) const = 0;
            virtual void render_filledTriangle(const std::vector<Point>& vertices, const Color& color) const = 0;
            virtual bool poll_event() = 0; 
            virtual void show() = 0;
            virtual void set_status(const std::string& status) = 0;  
            virtual void clear() = 0; 
            virtual ~RedtangleUI() {}; 
    }; 

    // Class to represent a piece 
    class Piece {
        protected:
            Color _team;
            std::vector<Color> _sides;
        public:
            Piece(Color team, std::vector<Color> sides) : _team(team), _sides(sides) {}
            bool is_piece() const { return !_sides.empty(); }
            const std::vector<Color>& get_sides() const  {return _sides; }
            const Color& get_team() const  {return _team; }
            virtual std::string get_type() const = 0;
            virtual void rotate(bool clockwise) = 0;
            virtual void render(const std::shared_ptr<RedtangleUI> ui, const Location& location) const = 0; 
    };

    // Implementation of piece to render a rectangle with no actual piece 
    class EmptyPiece : public Piece {
        public:
            EmptyPiece(Color team = NONE, std::vector<Color> sides = {}) : Piece(team, sides) {} 
            std::string get_type() const override { return "Empty"; }
            void rotate(bool clockwise) override {} 
            void render(const std::shared_ptr<RedtangleUI> ui, const Location& location) const override;  
    };

    // Implementation of piece to render an actual redtangle piece 
    // This class inherits from EmptyPiece so that it can utilize its rendering function 
    // RedtanglePiece::render() renders an empty piece and then renders the actual piece 
    class RedtanglePiece : public EmptyPiece {
        protected: 
            bool is_allBlack() const;
        public:
            RedtanglePiece(const Color& team, const std::vector<Color>& sides) : EmptyPiece(team, sides) {}
            std::string get_type() const override {return "Redtangle"; }
            void rotate(bool clockwise) override; 
            void render(const std::shared_ptr<RedtangleUI> ui, const Location& location) const override;
    }; 

    // Interface for playing a redtangle game 
    // The game can be ran locally on a computer with someone sharing a mouse 
    // Or can be ran remotely by connecting to a gRPC Server
    class Redtangle {
        public: 
            virtual bool select(const Location& piece_location) = 0; 
            virtual bool rotate(bool clockwise) = 0; 
            virtual bool end_turn() = 0; 
            virtual void render_board(const std::shared_ptr<RedtangleUI> ui) const = 0; 
            virtual ~Redtangle() = 0;
    }; 

    inline Redtangle::~Redtangle() {}; 

    // Implementation that performs the game logic and renders pieces to the ui
    class RedtangleGame : public Redtangle {
        protected:
            std::vector<std::vector<std::shared_ptr<Piece>>> _board; 
            Color _winner; 
            Color _turn;
            Location _curr_selection;
            int _black_pieces;
            int _white_pieces;  
            enum class GameState {
                SELECTING = 0,
                ROTATING = 1,
                JUMPING = 2,
                GAME_OVER = 3
            } _state;
            void create_board(const std::vector<std::vector<Color>>& orientations); 
            bool select_piece(const Location& location); 
            bool is_suicide(const Location& loc) const; 
            bool jump(Location curr, const Location& dest, const Color& team_sideColor, Side team_side, Side opp_side, int& pieces_captured);
            bool jump(const Location& location);
        public:
            RedtangleGame(); 
            bool select(const Location& location) override; 
            bool rotate(bool clockwise) override; 
            bool end_turn() override; 
            void render_board(const std::shared_ptr<RedtangleUI> ui) const override; 
            Color get_winner() const {return _winner; }
            Color get_turn() const { return _turn; }
            int get_white_pieces() const {return _white_pieces; }
            int get_black_pieces() const {return _black_pieces; }
            std::shared_ptr<Piece> get_piece(const Location& location); 
            ~RedtangleGame() override {}; 
    }; 
    struct StatusInfo {
        int white_pieces;
        int black_pieces; 
        std::string turn;
        std::string winner;
        std::string white_name;
        std::string black_name;
    };
    inline std::string to_str(const Color& c) { 
        if (c == redtangle::NONE) {
            return ""; 
        }
        else if (c == redtangle::WHITE) {
            return "White"; 
        }
        else if (c == redtangle::BLACK) {
            return "Black"; 
        }
        else {
            throw std::invalid_argument("Unable to convert color.");
        }
     }
    inline std::string format_statusInfo(const StatusInfo& info) {
        if (info.winner != "") {
            return info.winner + " won!";
        }
        std::string status; 
        status += info.white_name + " Pieces: " + std::to_string(info.white_pieces) + "\n";
        status += info.black_name + " Pieces: " + std::to_string(info.black_pieces) + "\n";
        status += info.turn + "'s Turn"; 
        return status; 
    }
}