//
// Created by Kevin Doveton on 11/03/2018.
//

#include "WebSocket.hpp"

WebSocket::WebSocket(uint16_t port) {

}

void WebSocket::On(int e, void (*f)()) {
  _eventFunc[e] = f;
}

int WebSocket::Handle(epoll_event e) {
  // TODO: change
  _eventFunc[MESSAGE]();
}

void WebSocket::AddClient(struct sockaddr_in client, int clientFd) {
  _connections.push_back(new WebSocketConnection(client, clientFd));
}

void WebSocket::Broadcast(std::string msg) {

}
