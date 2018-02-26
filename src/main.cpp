#include "main.hpp"

int main(int argc, char *argv[]) {
  unsigned int port = 3000; // default
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  if (port == 0) {
    std::cout << "Port must be non zero." << std::endl;
    return 1;
  }

  WebSocket websocket(port);
  return 0;
}
