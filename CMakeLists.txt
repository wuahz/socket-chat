# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)
# 设置项目名称
project(socket_server)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 指定头文件搜索路径
include_directories(./)

# 设置源文件
set(SOURCES 
    socket/socket.cpp
    socket/server_socket.cpp
    socket/client_socket.cpp
    utility/logger.cpp)

# 添加可执行文件
add_executable(server ${SOURCES} server.cpp)
add_executable(client ${SOURCES} client.cpp)
