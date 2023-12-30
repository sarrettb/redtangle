#include <time.h>
#include "game/redtangle.h"

bool valid_location(const redtangle::Location& loc);
bool is_eightAdj(const redtangle::Location& src, const redtangle::Location& dst);
bool is_diagnol(const redtangle::Location& src, const redtangle::Location& dst);
bool in_opponentsRedtangle(const redtangle::Location& loc, const redtangle::Color c); 
redtangle::Color get_opponent(const redtangle::Color& team);
void generate_possibleOrientations(std::vector<redtangle::Color>& curr_orientation, std::vector<std::vector<redtangle::Color>>& orientations, int i = 0); 
std::vector<std::vector<redtangle::Color>> generate_possibleOrientations(); 
redtangle::Location move(const redtangle::Location& src, redtangle::Side team_side);

// Create the local redtangle object
redtangle::RedtangleGame::RedtangleGame() : _winner(redtangle::NONE), _black_pieces(0), _white_pieces(0),_board(GRID_SIZE, std::vector<std::shared_ptr<Piece>>(GRID_SIZE)), 
                                            _turn(WHITE), _state(GameState::SELECTING), _curr_selection(INVALID) {
    create_board(generate_possibleOrientations()); 
}

// Construct the board of pieces 
// If the location is in the redtangle, then randomly pick the color of each side and construct the RedtanglePiece
// Else construct an empty piece 
void redtangle::RedtangleGame::create_board(const std::vector<std::vector<Color>>& orientations) {
    srand(time(NULL)); 
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (in_redtangle({x, y})) {
                _board[x][y] = std::make_shared<RedtanglePiece>(y <= 1 ? WHITE : BLACK, orientations[rand() % orientations.size()]);
                _board[x][y]->get_team() == WHITE ? _white_pieces++ : _black_pieces++; 
            }
            else {
                _board[x][y] = std::make_shared<EmptyPiece>();
            }
        }
    }
}

// Logic for selecting a square
bool redtangle::RedtangleGame::select_piece(const Location& loc) { 
    bool status = false; 
    bool moved = false;
    std::cout << _curr_selection << std::endl; 
    if (_board[loc.x][loc.y]->get_team() == _turn) { // piece selected 
        _curr_selection = loc; 
        status = true; 
    }
    else { // check for valid move 
        // Winning move cannot be diagnol 
        if (!valid_location(_curr_selection)) {
            status = false; 
        }
        else if (in_opponentsRedtangle(loc, _board[_curr_selection.x][_curr_selection.y]->get_team()) && is_diagnol(loc, _curr_selection)) {
            status = false; 
        }
        else if (_curr_selection != INVALID && is_eightAdj(_curr_selection, loc) && !_board[loc.x][loc.y]->is_piece() && _state != GameState::JUMPING) { // eight adjancent move
            std::swap(_board[loc.x][loc.y], _board[_curr_selection.x][_curr_selection.y]);
            _state = GameState::ROTATING;
            _curr_selection = loc; 
            status = true; 
            moved = true; 
        } 
        else if (jump(loc)) { // move is not eight-adjacent, verify valid jump 
            _state = GameState::JUMPING; 
            status = true;
            moved = true;
            if (is_suicide(loc)) { // if suicide then both pieces are lost
                _white_pieces--;
                _black_pieces--;
                _board[_curr_selection.x][_curr_selection.y] = std::make_shared<EmptyPiece>(); 
                _board[loc.x][loc.y] = std::make_shared<EmptyPiece>(); 
                end_turn(); 
            }
            else { // move piece to jumped location 
                std::swap(_board[loc.x][loc.y], _board[_curr_selection.x][_curr_selection.y]);
                _curr_selection = loc; 
            }
        }
        if (moved && in_redtangle(loc) && (_board[loc.x][loc.y]->get_team() == BLACK ? (loc.y == 0 || loc.y == 1) : (loc.y == 6 || loc.y == 7))) {
            _state = GameState::GAME_OVER; 
            _winner = _board[loc.x][loc.y]->get_team();
            _curr_selection = INVALID; 
        }
    }
    return status;
}

// Recursive function to simulate jumping the pieces
// For every recursive call check if current team_sideColor != opp_sideColor
// If all checks pass, on the recursive return set the board piece to an empty piece (simulating a capture)
bool redtangle::RedtangleGame::jump(Location curr, const Location& dest, const Color& team_sideColor, Side team_side, Side opp_side, int& pieces_captured) {
    if (curr == dest) {
        return true;
    } 
    if (!_board[curr.x][curr.y]->is_piece() || _board[curr.x][curr.y]->get_team() == _turn) {
        return false;
    }
    if (team_sideColor == _board[curr.x][curr.y]->get_sides()[(int) opp_side]) {
        return false;
    }
    Location next = move(curr, team_side);
    if (jump(next, dest, team_sideColor, team_side, opp_side, pieces_captured)) {
        _board[curr.x][curr.y]->get_team() == WHITE ? _white_pieces-- : _black_pieces--;
        _board[curr.x][curr.y] = std::make_shared<EmptyPiece>(); // piece captured 
        pieces_captured++; 
        return true;
    }
    return false; 
}

// Suicide move is jumping off the board to capture an opponents piece
// This is true if the destination location is a piece and it is on the edge of the board 
bool redtangle::RedtangleGame::is_suicide(const Location& loc) const {
    return _board[loc.x][loc.y]->is_piece() && (loc.x == 0 || loc.x == GRID_SIZE - 1 || loc.y == 0 || loc.y == GRID_SIZE - 1); 
}

// Calculate parameters and call recursive jump function 
bool redtangle::RedtangleGame::jump(const Location& location) {
    if ( _board[location.x][location.y]->is_piece() && !is_suicide(location)) {
        return false; // not valid jump 
    }
    Side team_side; // side to compare with opp_side
    Side opp_side; // the side to check if an opponent is blocking the jump 
    int x_dist = location.x - _curr_selection.x;
    int y_dist = location.y - _curr_selection.y; 
    int pieces_captured = 0;
    if (x_dist  != 0 && y_dist != 0) {
        return false; // diagnal jump not permitted 
    }
    if (x_dist != 0) {
        team_side = x_dist > 0 ? Side::RIGHT : Side::LEFT; 
        opp_side = x_dist > 0 ? Side::LEFT : Side::RIGHT; 
    }
    else {
        team_side = y_dist > 0 ? Side::BOTTOM : Side::TOP; 
        opp_side = y_dist > 0 ? Side::TOP : Side::BOTTOM;
    }  
    // Call recursive function to capture pieces if valid jump 
    return jump( move( _curr_selection, team_side), 
                 location, 
                 _board[_curr_selection.x][_curr_selection.y]->get_sides()[(int) team_side], 
                 team_side, 
                 opp_side,
                 pieces_captured
               ) && pieces_captured > 0;
}


// Filter user input based on game state 
// If in a valid state, then attempt to select or move a piece 
bool redtangle::RedtangleGame::select(const Location& loc) {
    if (valid_location(loc) && (_state == GameState::SELECTING || _state == GameState::JUMPING)) {
        return select_piece(loc); 
    }
    return false;
}

// End the current turn
// Turn can be only moved if a piece has been moved or rotated 
bool redtangle::RedtangleGame::end_turn() {
    if (_state > GameState::SELECTING && _state < GameState::GAME_OVER) {
          _turn = get_opponent(_turn); 
          _state = GameState::SELECTING; 
          _curr_selection = INVALID; 
          return true; 
    }
    return false;
}

bool redtangle::RedtangleGame::rotate(bool clockwise) {
    if (_curr_selection != INVALID && _state < GameState::JUMPING) {
        _board[_curr_selection.x][_curr_selection.y]->rotate(clockwise); 
        _state = GameState::ROTATING; 
        return true;
    }
    return false;
}

std::shared_ptr<redtangle::Piece> redtangle::RedtangleGame::get_piece(const Location& location) {
    if (!valid_location(location)) {
        throw std::invalid_argument("Location is invalid.\n");
    }
    return _board[location.x][location.y]; 
}

// Renders every piece to the screen 
void redtangle::RedtangleGame::render_board(const std::shared_ptr<RedtangleUI> ui) const {
    ui->clear(); 
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            _board[x][y]->render(ui, {x, y}); 
        }
    }
    redtangle::StatusInfo status_info = { _white_pieces, 
                                          _black_pieces,
                                          redtangle::to_str(_turn),
                                          _state == GameState::GAME_OVER ? redtangle::to_str(_winner) : "", 
                                          "White",
                                          "Black"
                                        }; 
    ui->set_status(format_statusInfo(status_info)); 
    ui->show(); 
}

// Verify Location is within bounds of grid 
bool valid_location(const redtangle::Location& loc) { 
    return loc.x >= 0 && loc.x < redtangle::GRID_SIZE && loc.y >= 0 && loc.y < redtangle::GRID_SIZE; 
}

// Check if current location is one of the eight locations next to it
bool is_eightAdj(const redtangle::Location& src, const redtangle::Location& dst) { 
    return std::abs(src.x - dst.x) <= 1 && std::abs(src.y - dst.y) <= 1; 
}

bool is_diagnol(const redtangle::Location& src, const redtangle::Location& dst) { 
    return std::abs(src.x - dst.x) == 1 && std::abs(src.y - dst.y) == 1; 
}

bool in_opponentsRedtangle(const redtangle::Location& loc, const redtangle::Color team_color) {
    if (redtangle::in_redtangle(loc)) {
        return team_color == redtangle::WHITE ? loc.y > 1 : loc.y <= 1; 
    }
    return false;
}

// Determine who the opponent is for current team
// Assumes team is either color of WHITE or BLACK
redtangle::Color get_opponent(const redtangle::Color& team) {
    return team == redtangle::WHITE ? redtangle::BLACK : redtangle::WHITE; 
}

// Recursively generate all possible orientations of pieces 
// T.C: O(2^N), S.C: O(N) for recursive stack 
void generate_possibleOrientations(std::vector<redtangle::Color>& curr_orientation, std::vector<std::vector<redtangle::Color>>& orientations, int i ) {
    if (i == curr_orientation.size()) {
        orientations.push_back(curr_orientation);
        return;
    }
    generate_possibleOrientations(curr_orientation, orientations, i + 1);
    curr_orientation[i] = redtangle::WHITE; 
    generate_possibleOrientations(curr_orientation, orientations, i + 1); 
    curr_orientation[i] = redtangle::BLACK; 
}

// Setup and call recursive function 
std::vector<std::vector<redtangle::Color>> generate_possibleOrientations() {
    using redtangle::BLACK, redtangle::Color; 
    std::vector<Color> curr_orientation = { BLACK, BLACK, BLACK, BLACK }; 
    std::vector<std::vector<Color>> orientations; 
    generate_possibleOrientations(curr_orientation, orientations); 
    return orientations; 
}

// Determine which direction to move towards, team_side is used to determine direction
redtangle::Location move(const redtangle::Location& src, redtangle::Side team_side) {
    switch (team_side) {
        case redtangle::Side::LEFT : {
            return { src.x - 1, src.y }; 
        }
        case redtangle::Side::TOP : {
            return { src.x, src.y - 1 }; 
        }
        case redtangle::Side::RIGHT : {
            return { src.x + 1, src.y };
        }
        case redtangle::Side::BOTTOM : {
            return { src.x, src.y + 1 }; 
        }
    }
}