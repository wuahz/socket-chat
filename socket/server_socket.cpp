#include <socket/server_socket.h>
using namespace wuah::socket;

ServerSocket::ServerSocket(const std::string& ip, int port) : Socket()
{
    set_recvbuf_size(10*1024);
    set_sendbuf_size(10*1024);
    set_linger(true, 10);
    set_keepalive(true);
    set_reuseaddr(true);

    bind(ip, port);
    listen(1024);
}