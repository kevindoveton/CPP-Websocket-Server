//
// Created by Kevin Doveton on 10/03/2018.
//

#ifndef CPP_WEB_SOCKET_WEBSOCKETCONNECTION_HPP
#define CPP_WEB_SOCKET_WEBSOCKETCONNECTION_HPP

#define WSGUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define MAX_FRAME_SIZE 2500

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <iostream>
#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <cryptlite/sha1.h>

#include "Http.hpp"
#include "WebSocketFrame.hpp"

class WebSocketConnection {
  public:
    WebSocketConnection(struct sockaddr_in client, int clientFd);
    ~WebSocketConnection();
    void CloseConnection();
    void AcceptConnection();
    std::string GetMessage();
    void Ping();
    void Pong();
    void SendMessage(std::string msg);

    void SendFrame(WebSocketFrame f);
    WebSocketFrame GetFrame();

    enum STATE { OPEN, CLOSING, CLOSED, UNKNOWN };

  protected:
    void closeSocket();

    bool _accepted;
    int _clientFd;
    struct sockaddr_in _client;

    /*
     * _state
     * The current state of the connection
    */
    int _state;
};


#endif //CPP_WEB_SOCKET_WEBSOCKETCONNECTION_HPP
