#include <iostream>
#include <memory>
#include "statusapis.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

class StatusClient {
public:
    StatusClient(std::shared_ptr<Channel> channel): m_stub(Greeter::NewStub(channel))
    {
        std::cout << __func__;
    }

    std::string SayHello(const std::string& message)
    {
        HelloRequest  request;
        request.set_name(message);
        HelloReply reply;
        ClientContext context;
        auto status = m_stub->SayHello(&context, request, &reply);

        if(status.ok())
        {
            return reply.message();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }
private:
    std::unique_ptr<Greeter::Stub> m_stub;
};

std::unique_ptr<StatusClient> initClient()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto serviceAddr = "localhost:50051";
    std::unique_ptr<StatusClient> client (new StatusClient(grpc::CreateChannel(serviceAddr, grpc::InsecureChannelCredentials())));
    return std::move(client);
}

int main(int argc, char *argv[])
{
    auto client = initClient();
    int t = 5;
    while (t > 0) {
        t--;
        std::cout << "Server says: " << client->SayHello("Oye bsdk!") << std::endl;
    }
    return 0;
}
