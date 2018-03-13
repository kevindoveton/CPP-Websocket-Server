# C++ Websocket Server

## API
### ws.On(event, handler)
### ws.Broadcast("Message to broadcast")

## Usage 
```c++
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

  Http http = Http(port, &ws);

  ev.AddHandler(http.GetFd(), &http, EPOLLIN);

  ws.On(WebSocket::MESSAGE, WebSocketEvents::onMsg);
  ws.On(WebSocket::OPEN, WebSocketEvents::onOpen);
  ws.On(WebSocket::CLOSE, WebSocketEvents::onClose);

  // this should be the last thing
  ev.Run();

  return 0;
}
```
