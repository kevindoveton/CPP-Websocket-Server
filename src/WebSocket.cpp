//
// Created by Kevin Doveton on 11/03/2018.
//

#include "WebSocket.hpp"

WebSocket::WebSocket(uint16_t port) {
  t = 1;
}

void WebSocket::On(int e, void (*f)()) {
  _eventFunc[e] = f;
}

int WebSocket::Handle(epoll_event e) {
  // TODO: change
  int fd = e.data.fd;

  if (e.events & EPOLLHUP) {

  } else {

    std::string t = _connections[ fd ]->GetMessage();
    if (t != "") {
      std::cout << "Msg: " << t << std::endl;
    }
  }

//  _eventFunc[MESSAGE]();
}

void WebSocket::AddClient(struct sockaddr_in client, int clientFd) {
  SetNonBlocking(clientFd);
  _connections[clientFd] = (new WebSocketConnection(client, clientFd));
  _ev->AddHandler(clientFd, this, EPOLLIN);
}

void WebSocket::Broadcast(std::string msg) {

}

WebSocket::~WebSocket() {
  std::map<int, WebSocketConnection*>::iterator it;
  for (it = _connections.begin(); it != _connections.end(); ++it) {
    delete it->second;
  }
}
