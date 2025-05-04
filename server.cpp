#include <iostream>
#include <socket/server_socket.h>
using namespace wuah::socket;

int main()
{
    Logger::getInstance()->open("../logs/server.log"); // 可执行文件在build目录下

    // set the address and port of the server
    string ip = "127.0.0.1";
    int port = 8080;
    
    // 1. create a server socket
    // 2. bind it to the address and port
    // 3. listen for incoming connections
    ServerSocket server(ip, port); 
    
    while(true)
    {
        // 4. accept incoming connections
        int connfd = server.accept();
        if(connfd < 0)
        {
            return -1;
        }

        // client socket
        Socket client(connfd); // communicate with the client

        char buf[1024];
        memset(buf, 0, sizeof(buf));

        // 5. receive data from the client
        int recv_len = client.recv(buf, sizeof(buf));
        if(recv_len > 0)
        {
            printf("recv: connfd=%d, msg=%s\n", connfd, buf);
        }

        // 6. send data to the client
        // memset(buffer, 0, sizeof(buffer));
        // sprintf(buffer, "Hello, client!");
        
        client.send(buf, recv_len);
    }

    // 7. close the socket
    // deconstructor of ServerSocket will call Socket::close()
    // server.close();

    return 0;
}