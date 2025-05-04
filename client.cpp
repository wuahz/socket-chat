#include <iostream>
#include <socket/client_socket.h>
using namespace wuah::socket;

int main()
{
    Logger::getInstance()->open("../logs/client.log"); // 可执行文件在build目录下
    
    // set the address and port of the server
    string ip = "127.0.0.1";
    int port = 8080;
    
    // 1. create a socket
    // 2. connect to the server
    ClientSocket client(ip, port);

    // 3. send data to the server
    string data = "hello, world!";
    client.send(data.c_str(), data.size());
    
    // 4. receive data from the server
    char buf[1024] = {0};
    client.recv(buf, sizeof(buf));
    cout << "recv data: " << buf << endl;

    // 7. close the socket
    // deconstructor of ClientSocket will call Socket::close()
    // client.close();

    return 0;
}