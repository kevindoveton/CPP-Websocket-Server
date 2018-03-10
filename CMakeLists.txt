cmake_minimum_required(VERSION 3.9)
project(CPP_Websocket_Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(CPP_Websocket_Server
        src/main.cpp
        src/main.hpp
        src/WebSocket.cpp
        src/WebSocket.hpp
        src/WebSocketFrame.cpp
        src/WebSocketFrame.hpp
        test/main.test.cpp
        test/WebSocket.test.cpp src/HttpServer.cpp src/HttpServer.h)
