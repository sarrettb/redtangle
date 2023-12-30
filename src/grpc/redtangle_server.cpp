#include "grpc/redtangle_server.h"
#include "game/redtangle.h"
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

redtangle_grpc::Color to_grpcColor(redtangle::Color c) { 
    return c == redtangle::WHITE ? redtangle_grpc::Color::WHITE : redtangle_grpc::Color::BLACK; 
}

grpc::Status RedtangleService::Connect(grpc::ServerContext* context, const redtangle_grpc::ConnectRequest* request, redtangle_grpc::ConnectResponse* response) {
    std::cout << "Received Connection Request: " << request->user_name() << std::endl;
    if (_games.empty() || _games.back()->white_name.empty()) {
        std::shared_ptr<GameData> new_game = std::make_shared<GameData>();
        new_game->white_name = request->user_name(); 
        _games.emplace_back(new_game);
        response->set_is_white(true); 
    }
    else {
        _games.back()->black_name = request->user_name();
        response->set_is_white(false);
    }
    response->set_game_id(_games.size() - 1); 
    std::cout << "Number of Games: " << _games.size() << std::endl; 
    return grpc::Status::OK; 
}

grpc::Status RedtangleService::Select(grpc::ServerContext* context, const redtangle_grpc::SelectRequest* request, redtangle_grpc::SelectResponse* response) {
    std::cout << "Received Select Request: " << request->user_name() << std::endl; 
    int game_id = request->game_id(); 
    if (game_id < 0 || game_id >= _games.size()) {
        throw std::invalid_argument("game_id is invalid.\n");
    }
    auto game = _games[game_id]; 
    std::string turn_name = game->redtangle.get_turn() == redtangle::WHITE ? game->white_name : game->black_name;  
    bool success = false;
    if (turn_name == request->user_name()) {
        success = game->redtangle.select({request->x(), request->y()}); 
    }
    response->set_success(success); 
    return grpc::Status::OK; 
}

grpc::Status RedtangleService::Rotate(grpc::ServerContext* context, const redtangle_grpc::RotateRequest* request, redtangle_grpc::RotateResponse* response) {
    int game_id = request->game_id(); 
    if (game_id < 0 || game_id >= _games.size()) {
        throw std::invalid_argument("game_id is invalid.\n");
    }
    auto game = _games[game_id]; 
    std::string turn_name = game->redtangle.get_turn() == redtangle::WHITE ? game->white_name : game->black_name; 
    bool success = false; 
    if (turn_name == request->user_name()) {
        success = game->redtangle.rotate(request->clockwise()); 
    }
    response->set_success(success); 
    return grpc::Status::OK; 
}

grpc::Status RedtangleService::EndTurn(grpc::ServerContext* context, const redtangle_grpc::EndTurnRequest* request, redtangle_grpc::EndTurnResponse* response) {
    int game_id = request->game_id(); 
    if (game_id < 0 || game_id >= _games.size()) {
        throw std::invalid_argument("game_id is invalid.\n");
    }
    auto game = _games[game_id]; 
    std::string turn_name = game->redtangle.get_turn() == redtangle::WHITE ? game->white_name : game->black_name;  
    bool success = false;
    if (turn_name == request->user_name()) {
        success = game->redtangle.end_turn(); 
    }
    response->set_success(success);  
    return grpc::Status::OK; 
}

grpc::Status RedtangleService::GetStatus(grpc::ServerContext* context, const redtangle_grpc::GetStatusRequest* request, redtangle_grpc::GetStatusResponse* response) {
    int game_id = request->game_id(); 
    if (game_id < 0 || game_id >= _games.size()) {
        std::cout << "Invalid Game ID!\n"; 
        throw std::invalid_argument("game_id is invalid.\n");
    }
    redtangle_grpc::Status game_status; 
    auto game = _games[game_id]; 
    std::string winner = redtangle::to_str(game->redtangle.get_winner()); 
    std::string opponent = request->user_name() == game->white_name ? game->black_name : game->white_name;
    game_status.set_winner(winner);
    game_status.set_opponent(opponent);
    game_status.set_turn(to_grpcColor(game->redtangle.get_turn())); 
    game_status.set_white_pieces(game->redtangle.get_white_pieces()); 
    game_status.set_black_pieces(game->redtangle.get_black_pieces()); 
    google::protobuf::RepeatedPtrField<redtangle_grpc::Piece> grpc_board; 
    for (int row = 0; row < redtangle::GRID_SIZE; row++) {
        for (int col = 0; col < redtangle::GRID_SIZE; col++) {
            auto piece = game->redtangle.get_piece({col, row}); 
            auto sides = piece->get_sides(); 
            auto grpc_piece = grpc_board.Add(); 
            grpc_piece->set_team_color(to_grpcColor(piece->get_team()));  
            google::protobuf::RepeatedField<int> grpc_sides; 
            for (auto c : sides) {
                auto color = grpc_sides.Add(); 
                *color = to_grpcColor(c);
            }
            grpc_piece->mutable_sides()->CopyFrom(grpc_sides); 
        }
    }  
    game_status.mutable_board()->CopyFrom(grpc_board); 
    response->mutable_game_status()->CopyFrom(game_status); 
    return grpc::Status::OK; 
}

grpc::Status RedtangleService::Disconnect(grpc::ServerContext* context, const redtangle_grpc::DisconnectRequest* request, redtangle_grpc::DisconnectResponse* response) {
    std::cout << "Disconnect Request: " << request->user_name() << std::endl; 
    int game_id = request->game_id(); 
    if (game_id < 0 || game_id >= _games.size()) {
        throw std::invalid_argument("game_id is invalid.\n");
    }
    auto game = _games[game_id]; 
    if (game->white_name == request->user_name()) {
        game->white_name.clear(); 
    }
    else if (game->black_name == request->user_name()) {
        game->black_name.clear();  
    }
    if (game->black_name.empty() && game->white_name.empty()) {
        _games.erase(_games.begin() + game_id); 
    }
    return grpc::Status::OK;  
}


RedtangleServer::RedtangleServer(int port, std::string address, std::shared_ptr<grpc::ServerCredentials> server_credentials) : 
_port(port), _ip_address(address), _creds(server_credentials) {}

RedtangleServer::~RedtangleServer() {
    _server->Shutdown(); 
}

void RedtangleServer::start_server() {
    std::string address = _ip_address + ':' + std::to_string(_port); 
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin(); 
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, _creds);
    builder.RegisterService(&_redtangle_service); 
    _server = builder.BuildAndStart();
    std::cout << "Server listening on port: " << address << std::endl;
}