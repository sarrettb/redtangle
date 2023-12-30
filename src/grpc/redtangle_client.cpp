#include "grpc/redtangle_client.h"

RedtangleClient::RedtangleClient(const std::string& user_name, 
                                 const std::string& ip_address, 
                                 int port, 
                                 std::shared_ptr<grpc::ChannelCredentials> creds
                                ) : _user_name(user_name)
{
    if (_user_name.empty()) {
        _user_name =  std::to_string((long long int) this); // If user name not specified set it to the address of the client object
    }
    std::string address = ip_address + ':' + std::to_string(port); 
    _stub = redtangle_grpc::Redtangle::NewStub(grpc::CreateChannel(address, creds)); 
    connect(); 
}

void RedtangleClient::connect() {
    redtangle_grpc::ConnectRequest request; 
    redtangle_grpc::ConnectResponse response;
    grpc::ClientContext context;
    request.set_user_name(_user_name);
    grpc::Status status = _stub->Connect(&context, request, &response);
    if (!status.ok()) {
        std::string error_msg = "RPC Failed: " + std::to_string(status.error_code()) + ":" + status.error_message();
        throw std::runtime_error(error_msg); 
    }
    _is_white_team = response.is_white();
    _game_id = response.game_id();  
} 

bool RedtangleClient::select(int x, int y) const {
    redtangle_grpc::SelectRequest request; 
    redtangle_grpc::SelectResponse response;
    grpc::ClientContext context;
    request.set_game_id(_game_id); 
    request.set_user_name(_user_name);
    request.set_x(x);
    request.set_y(y); 
    grpc::Status status = _stub->Select(&context, request, &response);
    if (!status.ok()) {
        std::string error_msg = "RPC Failed: " + std::to_string(status.error_code()) + ":" + status.error_message();
        throw std::runtime_error(error_msg); 
    }
    return response.success(); 
}

bool RedtangleClient::rotate(bool clockwise) const {
    redtangle_grpc::RotateRequest request; 
    redtangle_grpc::RotateResponse response;
    grpc::ClientContext context;
    request.set_game_id(_game_id); 
    request.set_user_name(_user_name);
    request.set_clockwise(clockwise);
    grpc::Status status = _stub->Rotate(&context, request, &response);
    if (!status.ok()) {
        std::string error_msg = "RPC Failed: " + std::to_string(status.error_code()) + ":" + status.error_message();
        throw std::runtime_error(error_msg); 
    }
    return response.success(); 
}

bool RedtangleClient::end_turn() const {
    redtangle_grpc::EndTurnRequest request; 
    redtangle_grpc::EndTurnResponse response;
    grpc::ClientContext context;
    request.set_game_id(_game_id); 
    request.set_user_name(_user_name);
    grpc::Status status = _stub->EndTurn(&context, request, &response);
    if (!status.ok()) {
        std::string error_msg = "RPC Failed: " + std::to_string(status.error_code()) + ":" + status.error_message();
        throw std::runtime_error(error_msg); 
    }
    return response.success(); 
}

redtangle_grpc::Status RedtangleClient::get_status() const {
    redtangle_grpc::GetStatusRequest request; 
    redtangle_grpc::GetStatusResponse response;
    grpc::ClientContext context;
    request.set_game_id(_game_id); 
    request.set_user_name(_user_name); 
    grpc::Status status = _stub->GetStatus(&context, request, &response);
    if (!status.ok()) {
        std::string error_msg = "RPC Failed: " + std::to_string(status.error_code()) + ":" + status.error_message();
        throw std::runtime_error(error_msg); 
    }
    return response.game_status(); 
}

void RedtangleClient::disconnect() {
    redtangle_grpc::DisconnectRequest request; 
    redtangle_grpc::DisconnectResponse response;
    grpc::ClientContext context;
    request.set_game_id(_game_id); 
    request.set_user_name(_user_name); 
    grpc::Status status = _stub->Disconnect(&context, request, &response);
    if (!status.ok()) {
        std::string error_msg = "RPC Failed: " + std::to_string(status.error_code()) + ":" + status.error_message();
        throw std::runtime_error(error_msg); 
    }
}

RedtangleClient::~RedtangleClient() {
    disconnect(); 
}
