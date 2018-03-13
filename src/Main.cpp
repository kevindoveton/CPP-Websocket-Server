//
// Created by Kevin Doveton on 10/03/2018.
//

#include "Main.hpp"

int main(int argc, char *argv[]) {
  int port = 3000; // default
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  if (port == 0) {
    std::cout << "Port must be non zero." << std::endl;
    return 1;
  }

  EventLoop ev;

  WebSocket ws = WebSocket(port);
  ws.SetEventLoopInstance(&ev);
  ws.On(WebSocket::MESSAGE, WebSocketEvents::onMsg);

  Http http = Http(port, &ws);

  ev.AddHandler(http.GetFd(), &http, EPOLLIN);

  ev.Run();

  return 0;
}
