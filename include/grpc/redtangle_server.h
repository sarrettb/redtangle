#pragma once
#include <string> 
#include "grpcpp/grpcpp.h"
#include "generated/redtangle.grpc.pb.h"
#include "game/redtangle.h"

class RedtangleService final : public redtangle_grpc::Redtangle::Service {
    private:
      struct GameData {
        redtangle::RedtangleGame redtangle; 
        std::string white_name;
        std::string black_name; 
      }; 
      std::vector<std::shared_ptr<GameData>> _games; 
      grpc::Status Connect(grpc::ServerContext* context, const redtangle_grpc::ConnectRequest* request, redtangle_grpc::ConnectResponse* response) override; 
      grpc::Status Select(grpc::ServerContext* context, const redtangle_grpc::SelectRequest* request, redtangle_grpc::SelectResponse* response) override;
      grpc::Status Rotate(grpc::ServerContext* context, const redtangle_grpc::RotateRequest* request, redtangle_grpc::RotateResponse* response) override;
      grpc::Status EndTurn(grpc::ServerContext* context, const redtangle_grpc::EndTurnRequest* request, redtangle_grpc::EndTurnResponse* response) override; 
      grpc::Status GetStatus(grpc::ServerContext* context, const redtangle_grpc::GetStatusRequest* request, redtangle_grpc::GetStatusResponse* response) override;
      grpc::Status Disconnect(grpc::ServerContext* context, const redtangle_grpc::DisconnectRequest* request, redtangle_grpc::DisconnectResponse* response) override;
};


class RedtangleServer {
  private:
    int _port; 
    std::string _ip_address; 
    RedtangleService _redtangle_service; 
    std::shared_ptr<grpc::ServerCredentials> _creds; 
    std::unique_ptr<grpc::Server> _server;

  public:
    RedtangleServer(int port = 50052, std::string address = "0.0.0.0", std::shared_ptr<grpc::ServerCredentials> server_credentials = grpc::InsecureServerCredentials()); 
    ~RedtangleServer();
    void start_server(); 
};
