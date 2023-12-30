#include <algorithm> 
#include <functional>
#include <cassert> 
#include "game/redtangle.h"

using redtangle::Rect;  
using redtangle::Circle;  
using redtangle::Point; 
using redtangle::Color; 

std::vector<Point> calculate_sidePoints(redtangle::Side side, const Rect& area); 
std::tuple<Circle, Circle, Rect> calculate_pieceAreas(const redtangle::Location& location, const std::shared_ptr<redtangle::RedtangleUI> ui); 

// Determine if every side of the piece is black and team color is black
bool redtangle::RedtanglePiece::is_allBlack() const {
    return std::all_of(_sides.cbegin(), _sides.cend(), [](const Color& side_color) {return side_color == BLACK; }) && _team == BLACK;
}

// Render all the sides of the piece and circles, call EmptyPiece::render to render background 
void redtangle::RedtanglePiece::render(const std::shared_ptr<RedtangleUI> ui, const Location& location) const {
    EmptyPiece::render(ui, location); 
    auto [inner_circle, outer_circle, piece_area] = calculate_pieceAreas(location, ui); 
    for (int i = 0; i < _sides.size(); i++) {
        ui->render_filledTriangle(calculate_sidePoints((Side) i, piece_area), _sides[i]);
    }
    ui->render_filledCircle(outer_circle, is_allBlack() ? WHITE : BLACK); 
    ui->render_filledCircle(inner_circle, _team);
}

// Rotate the elements in _sides array 
// Clockwise -> move each element to the right in the array
// Counterclockwise -> move each element to the left in the array 
void redtangle::RedtanglePiece::rotate(bool clockwise) {
    Color temp = clockwise ? _sides.back() : _sides.front(); 
    int i = clockwise ? 0 : _sides.size() - 1; 
    std::function<void(int&)> increment;
    // ternary operator not supported for lamdas
    if (clockwise) {
        increment = [] (int& i) { i++; };
    }
    else {
       increment = [] (int& i) { i--; };  
    }
    for (i; i < _sides.size(); increment(i)) {
        std::swap(temp, _sides[i]); 
    }
}

// Calculate the points for rendering a side 
std::vector<Point> calculate_sidePoints(redtangle::Side side, const Rect& area) {
    switch (side) {
        case redtangle::Side::LEFT: {
            return {
                        { area.top_left.x + area.w / 2, area.top_left.y + area.h / 2},
                        { area.top_left.x, area.top_left.y},
                        { area.top_left.x, area.top_left.y + area.h }
                   }; 
        }
        case redtangle::Side::RIGHT: {
            return  {
                        { area.top_left.x + area.w / 2, area.top_left.y + area.h / 2 },
                        { area.top_left.x + area.w, area.top_left.y },
                        { area.top_left.x + area.w, area.top_left.y + area.h }
                    };
        }
        case redtangle::Side::TOP: {
            return  {
                        { area.top_left.x + area.w / 2, area.top_left.y + area.h / 2},
                        { area.top_left.x, area.top_left.y},
                        { area.top_left.x + area.w, area.top_left.y}
                    };
        }
        case redtangle::Side::BOTTOM: {
            return {
                        { area.top_left.x + area.w / 2, area.top_left.y + area.h / 2},
                        { area.top_left.x, area.top_left.y + area.h },
                        { area.top_left.x + area.w, area.top_left.y + area.h}
                   };
        }
        default: {
            assert(("Invalid value given as a side in calculate_sidePoints ", true));
            return {}; 
        }
    }
}

// Calculate the inner circle, outer circle, and bounding rectangle of the piece 
std::tuple<Circle, Circle, Rect> calculate_pieceAreas(const redtangle::Location& location, const std::shared_ptr<redtangle::RedtangleUI> ui) {
    const int RECT_WIDTH = ui->get_rectWidth(); 
    const int RECT_HEIGHT = ui->get_rectHeight();
    const int EDGE_SIZE = ui->get_edgeSize(); 
    const int PIECE_WIDTH = ui->get_pieceWidth();
    const int PIECE_HEIGHT = ui->get_pieceHeight(); 
    const int INNER_R = ui->get_innerRadius();
    const int OUTER_R = ui->get_outerRadius(); 
    const Rect redtangle_surface = ui->get_redtangleSurface();
    Circle inner_circle = {
                            location.x * RECT_WIDTH + EDGE_SIZE + (RECT_WIDTH - EDGE_SIZE) / 2 + redtangle_surface.top_left.x,
                            location.y * RECT_HEIGHT + EDGE_SIZE + (RECT_HEIGHT - EDGE_SIZE) / 2 + redtangle_surface.top_left.y,
                            INNER_R
                          };
    Circle outer_circle = {inner_circle.center.x, inner_circle.center.y, OUTER_R}; 
    Rect piece_area = { 
                        inner_circle.center.x - PIECE_WIDTH / 2, 
                        inner_circle.center.y - PIECE_HEIGHT / 2, 
                        PIECE_WIDTH, 
                        PIECE_HEIGHT 
                      }; 
    return {inner_circle, outer_circle, piece_area }; 
}