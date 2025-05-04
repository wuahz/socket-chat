#include <socket/socket.h>
using namespace wuah::socket;

Socket::Socket() : m_ip(""), m_port(0), m_sockfd(0)
{
    // 1. create a socket
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(m_sockfd < 0)
    {
        LOG_ERROR("create socket error: errno=%d errmsg=%s\n", errno, strerror(errno));
    }
    LOG_DEBUG("create socket success, fd=%d\n", m_sockfd);
}

Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd)
{
    LOG_DEBUG("create socket success, fd=%d\n", m_sockfd);
}

Socket::~Socket()
{
    close();
}

bool Socket::bind(const string& ip, int port)
{
    // 2. bind the socket
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));  // 清空所有字段
    sockaddr.sin_family = AF_INET; // 设置协议族
    if(ip.empty())
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 设置IP地址为INADDR_ANY（0.0.0.0）
    }
    else
    {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str()); // 设置IP地址（网络字节序）
    }
    sockaddr.sin_port = htons(port); // 设置端口（网络字节序）

    // 2. bind the socket to the address and port
    if(::bind(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        LOG_ERROR("bind the socket error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    
    m_ip = ip;
    m_port = port;
    LOG_DEBUG("bind the socket success, ip:%s, port:%d\n", m_ip, m_port);
    return true;
}

bool Socket::listen(int backlog)
{
    // 3. listen for incoming connections
    if(::listen(m_sockfd, backlog) < 0)
    {
        LOG_ERROR("listen for incoming connections error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("listen for incoming connections success\n");
    return true;
}

int Socket::accept()
{
    int connfd = ::accept(m_sockfd, (struct sockaddr*)nullptr, nullptr);
    if(connfd < 0)
    {
        LOG_ERROR("accept incoming connections error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 0;
    }
    LOG_DEBUG("accept incoming connections success\n");
    return connfd;
}

bool Socket::connect(const string& ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));  // 清空所有字段
    sockaddr.sin_family = AF_INET; // 设置协议族
    if(ip.empty())
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 设置IP地址为INADDR_ANY（0.0.0.0）
    }
    else
    {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str()); // 设置IP地址（网络字节序）
    }
    sockaddr.sin_port = htons(port); // 设置端口（网络字节序）

    // 2. connect to the server
    if(::connect(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        LOG_ERROR("connect to the server error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }

    m_ip = ip;
    m_port = port;
    printf("connect to the server success\n");
    return true;
}

int Socket::send(const char* buf, int len)
{
    LOG_DEBUG("send: %s\n", buf);
    return ::send(m_sockfd, buf, len, 0);
}

int Socket::recv(char* buf, int len)
{
    LOG_DEBUG("recv: sockfd=%d, %s\n", m_sockfd, buf);
    return ::recv(m_sockfd, buf, len, 0);
}

void Socket::close()
{
    if(m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
        LOG_DEBUG("close the socket success\n");
    }
}

bool Socket::set_nonblocking()
{
    int flags = fcntl(m_sockfd, F_GETFL, 0); // 获取当前文件描述符的标志
    if(flags < 0)
    {
        LOG_ERROR("get socket flags error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    flags |= O_NONBLOCK; // 设置非阻塞标志
    fcntl(m_sockfd, F_SETFL, flags); // 设置文件描述符的标志
    if(flags < 0)
    {
        LOG_ERROR("set socket flags error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("set socket nonblocking success\n");

    return true;
}

bool Socket::set_sendbuf_size(int size)
{
    int buf_size = size;
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size)) < 0)
    {
        LOG_ERROR("set socket send buffer size error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("set socket send buffer size success, size=%d\n", size);
    return true;
}

bool Socket::set_recvbuf_size(int size)
{
    int buf_size = size;
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) < 0)
    {
        LOG_ERROR("set socket recv buffer size error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("set socket recv buffer size success, size=%d\n", size);
    return true;
}

bool Socket::set_linger(bool on, int seconds)
{
    struct linger linger;
    memset(&linger, 0, sizeof(linger)); // 清空所有字段
    linger.l_onoff = on ? 1 : 0; // 是否启用
    linger.l_linger = seconds; // 等待时间（单位：秒）
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger)) < 0)
    {
        LOG_ERROR("set socket linger error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("set socket linger success, on=%d, seconds=%d\n", on, seconds);
    return true;
}

bool Socket::set_keepalive(bool on)
{
    int keepalive = on ? 1 : 0; // 是否启用
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0)
    {
        LOG_ERROR("set socket keepalive error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("set socket keepalive success, on=%d\n", on);
    return true;
}

bool Socket::set_reuseaddr(bool on)
{
    int reuseaddr = on ? 1 : 0; // 是否启用
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0)
    {
        LOG_ERROR("set socket reuseaddr error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    LOG_DEBUG("set socket reuseaddr success, on=%d\n", on);
    return true;
}