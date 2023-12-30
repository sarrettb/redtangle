#include "game/redtangle_remote.h" 

redtangle::Color to_redtangleColor(redtangle_grpc::Color grpc_color) {
    return grpc_color == redtangle_grpc::Color::WHITE ? redtangle::WHITE : redtangle::BLACK; 
}

std::string color(redtangle::Color color) {
    return color == redtangle::WHITE ? "White" : "Black"; 
}

RedtangleRemote::RedtangleRemote(const std::string& user_name, const std::string& ip_address, int port) : _client(user_name, ip_address, port) {}

bool RedtangleRemote::select(const redtangle::Location& piece_location) {
    return _client.select(piece_location.x, piece_location.y); 
}

bool RedtangleRemote::rotate(bool clockwise) {
    return _client.rotate(clockwise);
}

bool RedtangleRemote::end_turn() {
    return _client.end_turn(); 
}

void RedtangleRemote::render_board(const std::shared_ptr<redtangle::RedtangleUI> ui) const {
    redtangle_grpc::Status status = _client.get_status(); 
    auto grpc_board = status.board(); 
    ui->clear(); 
    int index = 0;
    std::shared_ptr<redtangle::Piece> piece; 
    for (int row = 0; row < redtangle::GRID_SIZE; row++) {
        for (int col = 0; col < redtangle::GRID_SIZE; col++) { 
            redtangle_grpc::Piece grpc_piece = grpc_board.Get(index++); 
            redtangle::Color team_color = to_redtangleColor(static_cast<redtangle_grpc::Color>(grpc_piece.team_color())); 
            auto grpc_sides = grpc_piece.sides(); 
            if (grpc_sides.size() == 0) {
                piece = std::make_shared<redtangle::EmptyPiece>();
            }
            else {
                std::vector<redtangle::Color> sides;
                for (int i = 0; i < grpc_sides.size(); i++) {
                   sides.push_back((to_redtangleColor(static_cast<redtangle_grpc::Color>(grpc_sides.Get(i))))); 
                }
                piece = std::make_shared<redtangle::RedtanglePiece>(team_color, sides);
            }
            piece->render(ui, {col, row}); 
        }
    }
    std::string white_name = _client.is_white_team() ? _client.get_user_name() : status.opponent();
    std::string black_name = _client.is_white_team() ? status.opponent() : _client.get_user_name();
    std::string winner = status.winner(); 
    if (black_name.empty()) {
        black_name = "No Player";
    }
    if (!winner.empty()) {
        winner = winner == "White" ? white_name : black_name; 
    }
    redtangle::StatusInfo status_info = { status.white_pieces(),
                                          status.black_pieces(),
                                          (status.turn() == redtangle_grpc::Color::WHITE ? white_name : black_name),
                                          winner, 
                                          white_name + " (White)",
                                          black_name + " (Black)"
                                        }; 
    ui->set_status(format_statusInfo(status_info)); 
    ui->show(); 
}