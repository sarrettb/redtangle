// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: redtangle.proto

#include "redtangle.pb.h"
#include "redtangle.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace redtangle_grpc {

static const char* Redtangle_method_names[] = {
  "/redtangle_grpc.Redtangle/Connect",
  "/redtangle_grpc.Redtangle/Select",
  "/redtangle_grpc.Redtangle/Rotate",
  "/redtangle_grpc.Redtangle/EndTurn",
  "/redtangle_grpc.Redtangle/GetStatus",
  "/redtangle_grpc.Redtangle/Disconnect",
};

std::unique_ptr< Redtangle::Stub> Redtangle::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Redtangle::Stub> stub(new Redtangle::Stub(channel, options));
  return stub;
}

Redtangle::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Connect_(Redtangle_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Select_(Redtangle_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Rotate_(Redtangle_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_EndTurn_(Redtangle_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetStatus_(Redtangle_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Disconnect_(Redtangle_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Redtangle::Stub::Connect(::grpc::ClientContext* context, const ::redtangle_grpc::ConnectRequest& request, ::redtangle_grpc::ConnectResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::redtangle_grpc::ConnectRequest, ::redtangle_grpc::ConnectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Connect_, context, request, response);
}

void Redtangle::Stub::async::Connect(::grpc::ClientContext* context, const ::redtangle_grpc::ConnectRequest* request, ::redtangle_grpc::ConnectResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::redtangle_grpc::ConnectRequest, ::redtangle_grpc::ConnectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Connect_, context, request, response, std::move(f));
}

void Redtangle::Stub::async::Connect(::grpc::ClientContext* context, const ::redtangle_grpc::ConnectRequest* request, ::redtangle_grpc::ConnectResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Connect_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::ConnectResponse>* Redtangle::Stub::PrepareAsyncConnectRaw(::grpc::ClientContext* context, const ::redtangle_grpc::ConnectRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::redtangle_grpc::ConnectResponse, ::redtangle_grpc::ConnectRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Connect_, context, request);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::ConnectResponse>* Redtangle::Stub::AsyncConnectRaw(::grpc::ClientContext* context, const ::redtangle_grpc::ConnectRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncConnectRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Redtangle::Stub::Select(::grpc::ClientContext* context, const ::redtangle_grpc::SelectRequest& request, ::redtangle_grpc::SelectResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::redtangle_grpc::SelectRequest, ::redtangle_grpc::SelectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Select_, context, request, response);
}

void Redtangle::Stub::async::Select(::grpc::ClientContext* context, const ::redtangle_grpc::SelectRequest* request, ::redtangle_grpc::SelectResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::redtangle_grpc::SelectRequest, ::redtangle_grpc::SelectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Select_, context, request, response, std::move(f));
}

void Redtangle::Stub::async::Select(::grpc::ClientContext* context, const ::redtangle_grpc::SelectRequest* request, ::redtangle_grpc::SelectResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Select_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::SelectResponse>* Redtangle::Stub::PrepareAsyncSelectRaw(::grpc::ClientContext* context, const ::redtangle_grpc::SelectRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::redtangle_grpc::SelectResponse, ::redtangle_grpc::SelectRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Select_, context, request);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::SelectResponse>* Redtangle::Stub::AsyncSelectRaw(::grpc::ClientContext* context, const ::redtangle_grpc::SelectRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSelectRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Redtangle::Stub::Rotate(::grpc::ClientContext* context, const ::redtangle_grpc::RotateRequest& request, ::redtangle_grpc::RotateResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::redtangle_grpc::RotateRequest, ::redtangle_grpc::RotateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Rotate_, context, request, response);
}

void Redtangle::Stub::async::Rotate(::grpc::ClientContext* context, const ::redtangle_grpc::RotateRequest* request, ::redtangle_grpc::RotateResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::redtangle_grpc::RotateRequest, ::redtangle_grpc::RotateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Rotate_, context, request, response, std::move(f));
}

void Redtangle::Stub::async::Rotate(::grpc::ClientContext* context, const ::redtangle_grpc::RotateRequest* request, ::redtangle_grpc::RotateResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Rotate_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::RotateResponse>* Redtangle::Stub::PrepareAsyncRotateRaw(::grpc::ClientContext* context, const ::redtangle_grpc::RotateRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::redtangle_grpc::RotateResponse, ::redtangle_grpc::RotateRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Rotate_, context, request);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::RotateResponse>* Redtangle::Stub::AsyncRotateRaw(::grpc::ClientContext* context, const ::redtangle_grpc::RotateRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRotateRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Redtangle::Stub::EndTurn(::grpc::ClientContext* context, const ::redtangle_grpc::EndTurnRequest& request, ::redtangle_grpc::EndTurnResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::redtangle_grpc::EndTurnRequest, ::redtangle_grpc::EndTurnResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_EndTurn_, context, request, response);
}

void Redtangle::Stub::async::EndTurn(::grpc::ClientContext* context, const ::redtangle_grpc::EndTurnRequest* request, ::redtangle_grpc::EndTurnResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::redtangle_grpc::EndTurnRequest, ::redtangle_grpc::EndTurnResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_EndTurn_, context, request, response, std::move(f));
}

void Redtangle::Stub::async::EndTurn(::grpc::ClientContext* context, const ::redtangle_grpc::EndTurnRequest* request, ::redtangle_grpc::EndTurnResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_EndTurn_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::EndTurnResponse>* Redtangle::Stub::PrepareAsyncEndTurnRaw(::grpc::ClientContext* context, const ::redtangle_grpc::EndTurnRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::redtangle_grpc::EndTurnResponse, ::redtangle_grpc::EndTurnRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_EndTurn_, context, request);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::EndTurnResponse>* Redtangle::Stub::AsyncEndTurnRaw(::grpc::ClientContext* context, const ::redtangle_grpc::EndTurnRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncEndTurnRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Redtangle::Stub::GetStatus(::grpc::ClientContext* context, const ::redtangle_grpc::GetStatusRequest& request, ::redtangle_grpc::GetStatusResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::redtangle_grpc::GetStatusRequest, ::redtangle_grpc::GetStatusResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetStatus_, context, request, response);
}

void Redtangle::Stub::async::GetStatus(::grpc::ClientContext* context, const ::redtangle_grpc::GetStatusRequest* request, ::redtangle_grpc::GetStatusResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::redtangle_grpc::GetStatusRequest, ::redtangle_grpc::GetStatusResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetStatus_, context, request, response, std::move(f));
}

void Redtangle::Stub::async::GetStatus(::grpc::ClientContext* context, const ::redtangle_grpc::GetStatusRequest* request, ::redtangle_grpc::GetStatusResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetStatus_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::GetStatusResponse>* Redtangle::Stub::PrepareAsyncGetStatusRaw(::grpc::ClientContext* context, const ::redtangle_grpc::GetStatusRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::redtangle_grpc::GetStatusResponse, ::redtangle_grpc::GetStatusRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetStatus_, context, request);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::GetStatusResponse>* Redtangle::Stub::AsyncGetStatusRaw(::grpc::ClientContext* context, const ::redtangle_grpc::GetStatusRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetStatusRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Redtangle::Stub::Disconnect(::grpc::ClientContext* context, const ::redtangle_grpc::DisconnectRequest& request, ::redtangle_grpc::DisconnectResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::redtangle_grpc::DisconnectRequest, ::redtangle_grpc::DisconnectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Disconnect_, context, request, response);
}

void Redtangle::Stub::async::Disconnect(::grpc::ClientContext* context, const ::redtangle_grpc::DisconnectRequest* request, ::redtangle_grpc::DisconnectResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::redtangle_grpc::DisconnectRequest, ::redtangle_grpc::DisconnectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Disconnect_, context, request, response, std::move(f));
}

void Redtangle::Stub::async::Disconnect(::grpc::ClientContext* context, const ::redtangle_grpc::DisconnectRequest* request, ::redtangle_grpc::DisconnectResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Disconnect_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::DisconnectResponse>* Redtangle::Stub::PrepareAsyncDisconnectRaw(::grpc::ClientContext* context, const ::redtangle_grpc::DisconnectRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::redtangle_grpc::DisconnectResponse, ::redtangle_grpc::DisconnectRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Disconnect_, context, request);
}

::grpc::ClientAsyncResponseReader< ::redtangle_grpc::DisconnectResponse>* Redtangle::Stub::AsyncDisconnectRaw(::grpc::ClientContext* context, const ::redtangle_grpc::DisconnectRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDisconnectRaw(context, request, cq);
  result->StartCall();
  return result;
}

Redtangle::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Redtangle_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Redtangle::Service, ::redtangle_grpc::ConnectRequest, ::redtangle_grpc::ConnectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Redtangle::Service* service,
             ::grpc::ServerContext* ctx,
             const ::redtangle_grpc::ConnectRequest* req,
             ::redtangle_grpc::ConnectResponse* resp) {
               return service->Connect(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Redtangle_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Redtangle::Service, ::redtangle_grpc::SelectRequest, ::redtangle_grpc::SelectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Redtangle::Service* service,
             ::grpc::ServerContext* ctx,
             const ::redtangle_grpc::SelectRequest* req,
             ::redtangle_grpc::SelectResponse* resp) {
               return service->Select(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Redtangle_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Redtangle::Service, ::redtangle_grpc::RotateRequest, ::redtangle_grpc::RotateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Redtangle::Service* service,
             ::grpc::ServerContext* ctx,
             const ::redtangle_grpc::RotateRequest* req,
             ::redtangle_grpc::RotateResponse* resp) {
               return service->Rotate(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Redtangle_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Redtangle::Service, ::redtangle_grpc::EndTurnRequest, ::redtangle_grpc::EndTurnResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Redtangle::Service* service,
             ::grpc::ServerContext* ctx,
             const ::redtangle_grpc::EndTurnRequest* req,
             ::redtangle_grpc::EndTurnResponse* resp) {
               return service->EndTurn(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Redtangle_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Redtangle::Service, ::redtangle_grpc::GetStatusRequest, ::redtangle_grpc::GetStatusResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Redtangle::Service* service,
             ::grpc::ServerContext* ctx,
             const ::redtangle_grpc::GetStatusRequest* req,
             ::redtangle_grpc::GetStatusResponse* resp) {
               return service->GetStatus(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Redtangle_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Redtangle::Service, ::redtangle_grpc::DisconnectRequest, ::redtangle_grpc::DisconnectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Redtangle::Service* service,
             ::grpc::ServerContext* ctx,
             const ::redtangle_grpc::DisconnectRequest* req,
             ::redtangle_grpc::DisconnectResponse* resp) {
               return service->Disconnect(ctx, req, resp);
             }, this)));
}

Redtangle::Service::~Service() {
}

::grpc::Status Redtangle::Service::Connect(::grpc::ServerContext* context, const ::redtangle_grpc::ConnectRequest* request, ::redtangle_grpc::ConnectResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Redtangle::Service::Select(::grpc::ServerContext* context, const ::redtangle_grpc::SelectRequest* request, ::redtangle_grpc::SelectResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Redtangle::Service::Rotate(::grpc::ServerContext* context, const ::redtangle_grpc::RotateRequest* request, ::redtangle_grpc::RotateResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Redtangle::Service::EndTurn(::grpc::ServerContext* context, const ::redtangle_grpc::EndTurnRequest* request, ::redtangle_grpc::EndTurnResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Redtangle::Service::GetStatus(::grpc::ServerContext* context, const ::redtangle_grpc::GetStatusRequest* request, ::redtangle_grpc::GetStatusResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Redtangle::Service::Disconnect(::grpc::ServerContext* context, const ::redtangle_grpc::DisconnectRequest* request, ::redtangle_grpc::DisconnectResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace redtangle_grpc

