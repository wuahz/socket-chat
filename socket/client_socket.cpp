#include <socket/client_socket.h>
using namespace wuah::socket;

ClientSocket::ClientSocket(const std::string& ip, int port) : Socket()
{
    connect(ip, port);
}