#pragma once  
#include "game/redtangle.h"
#include "grpc/redtangle_client.h"

// Implements the Redtangle Interface 
// 1. Sends gRPC Commands to the Server 
// 2. gRPC Responses used to render UI 
class RedtangleRemote : public redtangle::Redtangle {
    protected: 
        RedtangleClient _client; 
    
    public:
        RedtangleRemote(const std::string& user_name, const std::string& ip_address = "0.0.0.0", int port = 50052);
        bool select(const redtangle::Location& piece_location) override;
        bool rotate(bool clockwise) override;
        bool end_turn() override; 
        void render_board(const std::shared_ptr<redtangle::RedtangleUI> ui) const override; 
        ~RedtangleRemote() override {}
}; 