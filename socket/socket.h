#pragma once

#include <cstring> // for memset()
#include <sys/socket.h>  // for socket(), bind(), listen(), accept(), connect(), send(), recv(), close()
#include <netinet/in.h>  // for sockaddr_in structure
#include <unistd.h>      // for close(), read(), write()
#include <arpa/inet.h>   // for inet_addr()
#include <fcntl.h>       // for fcntl()
#include <string> // for string
#include <utility/Logger.h> // for Logger class
using namespace wuah::logger;

namespace wuah
{
namespace socket
{

class Socket
{
public:
    Socket(); // 创建套接字
    Socket(int sockfd); // 绑定已有套接字
    virtual ~Socket(); // 关闭套接字

    bool bind(const string& ip, int port); // 绑定地址和端口
    bool listen(int backlog); // 监听连接
    int accept(); // 接受连接
    bool connect(const string& ip, int port); // 连接到服务器
    int send(const char* buf, int len); // 发送数据
    int recv(char* buf, int len); // 接收数据
    void close(); // 关闭套接字

    bool set_nonblocking(); // 设置非阻塞模式
    bool set_sendbuf_size(int size); // 设置发送缓冲区大小
    bool set_recvbuf_size(int size); // 设置接收缓冲区大小
    bool set_linger(bool on, int seconds); // 设置延迟发送
    bool set_keepalive(bool on); // 设置保持连接
    bool set_reuseaddr(bool on); // 设置重用地址

protected:
    std::string m_ip;
    int m_port;
    int m_sockfd;
};

} // namespace socket
} // namespace wuah