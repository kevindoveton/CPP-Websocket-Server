//
// Created by Kevin Doveton on 11/03/2018.
//

#ifndef CPP_WEB_SOCKET_WEBSOCKET_HPP
#define CPP_WEB_SOCKET_WEBSOCKET_HPP


#include <cstdint>
#include <stdint-gcc.h>
#include <map>
#include "Http.hpp"
#include "Handler.hpp"

class Handler;

class WebSocket : public Handler {
  public:
    explicit WebSocket(uint16_t port);
    void On(int e, void (*f)());
    void Broadcast(std::string msg);
    void AddClient( struct sockaddr_in client, int client_sock);
    int Handle(epoll_event e) override;
    enum { OPEN, CLOSE, MESSAGE };

  protected:
    std::map <int, void(*)()> _eventFunc;
    std::vector<WebSocketConnection*> _connections;
};


#endif //CPP_WEB_SOCKET_WEBSOCKET_HPP
