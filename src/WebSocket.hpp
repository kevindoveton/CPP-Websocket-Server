#ifndef KDWEBSOCKET_H
#define KDWEBSOCKET_H

#define WSGUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

// system
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

// libs
#include <cryptlite/sha1.h>

// ours
#include "WebSocketFrame.hpp"

class WebSocket {
  public:
    explicit WebSocket(int port);
  private:
    /*
     * quite possibly the worst implementation ever
     * parse the http headers
     * return a map<string, string>
    */
    std::map<std::string, std::string> parseHttpHeaders(char *msg, int size);
    void readFrame(std::string frame);
    bool getBit(unsigned char byte, int position);

    int _port;
    int _sockfd;
    struct sockaddr_in _server;
};

#endif