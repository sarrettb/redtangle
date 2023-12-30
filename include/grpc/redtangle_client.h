#pragma once 
#include <memory> 
#include <string> 
#include <grpcpp/grpcpp.h>
#include "generated/redtangle.grpc.pb.h"

class RedtangleClient {
    private:
        std::unique_ptr<redtangle_grpc::Redtangle::Stub> _stub; 
        int _game_id;
        std::string _user_name; 
        bool _is_white_team;
        void connect();   
        void disconnect();
    public:
        RedtangleClient(const std::string& user_name,
                        const std::string& ip_address = "0.0.0.0",
                        int port = 50052, 
                        std::shared_ptr<grpc::ChannelCredentials> creds = grpc::InsecureChannelCredentials()
                       ); 
        bool select(int x, int y) const; 
        bool rotate(bool clockwise) const; 
        bool end_turn() const;  
        bool is_white_team() const { return _is_white_team; }
        std::string get_user_name() const {return _user_name; }
        redtangle_grpc::Status get_status() const; 
        ~RedtangleClient(); 
}; 
    