//
// Created by Kevin Doveton on 10/03/2018.
//

#include "Http.hpp"

Http::Http(uint16_t port, WebSocket *ws) {
  _port = port;
  _ws = ws;

  // set up socket
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd == -1) {
    std::cout << "Unable to create socket" << std::endl;
    exit(1);
  }

  std::cout << "Created socket" << std::endl;

  _server.sin_family = AF_INET;
  _server.sin_addr.s_addr = INADDR_ANY;
  _server.sin_port = htons(_port);

  //Bind
  if( bind(_sockfd,(struct sockaddr *)&_server , sizeof(_server)) < 0)
  {
    //print the error message
    std::cerr << "Unable to bind to port" << std::endl;
    exit(1);
  }

  std::cout << "Bind successful" << std::endl;

  // start listening
  listen(_sockfd , 3);
  std::cout << "Waiting for connection" << std::endl;

  _sizeofSock = sizeof(struct sockaddr_in);

  SetNonBlocking(GetFd());
}

Http::~Http() {

}

std::map<std::string, std::string> Http::ParseHttpHeaders(char *msg, ssize_t readSize) {
  std::map<std::string, std::string> header;

  std::string key = "";
  std::string val = "";

  int count = 0;

  // get the method
  while (true) {
    key.push_back(msg[count]);
    if (msg[count] == ' ') {
      count++;
      break;
    }
    count++;
  }

  header["METHOD"] = key;
  key.clear();

  while (true) {
    key.push_back(msg[count]);
    if (msg[count] == ' ') {
      count++;
      break;
    }
    count++;
  }

  header["PATH"] = key;
  key.clear();

  while (true) {
    key.push_back(msg[count]);
    if (msg[count] == '\r') {
      count++;
      count++; // skip the \n
      break;
    }
    count++;
  }

  header["VERSION"] = key;
  key.clear();

  bool isHeader = true;

  for (; count < readSize; count++) {
    if (isHeader) {
      if (msg[count] == ':') {
        isHeader = false;
        count++;
      } else {
        key.push_back(msg[count]);
      }
    } else {
      if (msg[count] == '\r') {
        count++; // skip the \n
        isHeader = true;
        header[key] = val;
        key.clear();
        val.clear();
      } else {
        val.push_back(msg[count]);
      }
    }
  }

//  for (const auto &p : header) {
//    std::cout << "m[" << p.first << "] = " << p.second << '\n';
//  }

  return header;
}


int Http::Handle(epoll_event e) {
  struct sockaddr_in client;
  int clientFd = accept(_sockfd, (struct sockaddr *) &client, (socklen_t *) &_sizeofSock);
  std::cout << "client: "  << clientFd << std::endl;

  if (clientFd < 0) {
    std::cerr << "Accept failed" << std::endl;
    exit(1);
  }

  std::cout << "Connection accepted" << std::endl;

  _ws->AddClient(client, clientFd);

  return 0;
};
