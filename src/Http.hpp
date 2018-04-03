//
// Created by Kevin Doveton on 10/03/2018.
//

#ifndef CPP_WEB_SOCKET_HTTP_HPP
#define CPP_WEB_SOCKET_HTTP_HPP

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

#include "WebSocket.hpp"
#include "WebSocketConnection.hpp"
#include "Handler.hpp"

class Handler;
class WebSocket;
class WebSocketConnection;

class Http : public Handler {
  public:
    explicit Http(uint16_t port, WebSocket *ws);
    ~Http();
    static std::map<std::string, std::string> ParseHttpHeaders(char *msg, ssize_t readSize);
    int GetFd() { return _sockfd; }
    int Handle(Handle_Event_t e) override;

  protected:
    uint16_t _port;
    int _sockfd;
    struct sockaddr_in _server;
    int _sizeofSock;
    WebSocket *_ws;
};


#endif //CPP_WEB_SOCKET_HTTP_HPP
