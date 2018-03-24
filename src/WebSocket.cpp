//
// Created by Kevin Doveton on 11/03/2018.
//

#include "WebSocket.hpp"

WebSocket::WebSocket(uint16_t port) {
}

void WebSocket::On(int e, WS_CB_FUNC f) {
  _eventFunc[e] = f;
}

int WebSocket::Handle(epoll_event e) {
  // TODO: change
  int fd = e.data.fd;
  WebSocketFrame f;
  if (e.events & EPOLLHUP) {

  } else {
    f = _connections[ fd ]->GetFrame();
  }

  HandlerEvent_t ev;
  ev.epollEvent = e;
  ev.wsFrame = f;
  ev.ws = this;
  ev.wsConnection = _connections[fd];

  int eventType = MESSAGE;
  if (ev.wsFrame.GetOpcode() == WebSocketFrame::CLOSE) {
    eventType = CLOSE;
  }

  auto func = _eventFunc.find(eventType);
  if (func != _eventFunc.end()) {
    func->second(ev);
  }

  switch(eventType) {
    case CLOSE: {
      // close the connection
      ev.wsConnection->CloseConnection();

      // clean it up
      auto it = _connections.find(fd);
      if (it != _connections.end()) {
        delete it->second;
        _connections.erase(it);
      }
       // no need to unregister, closing is enough
      break;
    }

    default:
      break;
  }
}

void WebSocket::AddClient(struct sockaddr_in client, int clientFd) {
  SetNonBlocking(clientFd);
  _connections[clientFd] = (new WebSocketConnection(client, clientFd));
  _ev->AddHandler(clientFd, this, EPOLLIN);
}

void WebSocket::Broadcast(std::string msg) {
  for (auto it = _connections.begin(); it != _connections.end(); ++it) {
    it->second->SendMessage(msg);
  }
}

WebSocket::~WebSocket() {
  for (auto it = _connections.begin(); it != _connections.end(); ++it) {
    delete it->second;
  }
}
