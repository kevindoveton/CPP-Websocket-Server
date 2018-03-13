cmake_minimum_required(VERSION 3.5)
project(CPP_Web_Socket)

set(CMAKE_CXX_STANDARD 11)

include_directories( libs )

add_executable(CPP_Web_Socket
        src/Main.cpp
        src/Main.hpp
        src/WebSocketConnection.cpp
        src/WebSocketConnection.hpp
        src/WebSocketFrame.cpp
        src/WebSocketFrame.hpp
        src/Http.cpp
        src/Http.hpp
        src/Utils.hpp src/WebSocket.cpp src/WebSocket.hpp src/EventLoop.cpp src/EventLoop.hpp src/Handler.hpp src/WebSocketEvents.cpp src/WebSocketEvents.hpp)