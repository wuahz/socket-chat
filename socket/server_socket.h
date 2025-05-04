#pragma once

#include <socket/socket.h>

namespace wuah 
{
namespace socket 
{

class ServerSocket : public Socket
{
public:
    ServerSocket() = delete;
    ServerSocket(const std::string& ip, int port);
    ~ServerSocket() = default;

};

} // namespace socket
} // namespace wuah
