#include "WebSocket.h"

WebSocket::WebSocket(int port) {
  _port = port;

  // set up socket
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd == -1) {
    std::cout << "Unable to create socket" << std::endl;
    exit(1);
  }

  std::cout << "Created socket" << std::endl;

  _server.sin_family = AF_INET;
  _server.sin_addr.s_addr = INADDR_ANY;
  _server.sin_port = htons(port);

  //Bind
  if( bind(_sockfd,(struct sockaddr *)&_server , sizeof(_server)) < 0)
  {
      //print the error message
      std::cout << "Unable to bind to port" << std::endl;
      exit(1);
  }

  std::cout << "Bind successful" << std::endl;

  listen(_sockfd , 3);
  std::cout << "Waiting for connection" << std::endl;
  int c = sizeof(struct sockaddr_in);

  //accept connection from an incoming client
  struct sockaddr_in client;
  char client_message[2500];
  int read_size;
  int client_sock = accept(_sockfd, (struct sockaddr *)&client, (socklen_t*)&c);
  if (client_sock < 0)
  {
    perror("accept failed");
    exit( 1 );
  }
  puts("Connection accepted");

  //Receive a message from client
  while( (read_size = read(client_sock , client_message , sizeof(client_message))) > 0 )
  {
    //Send the message back to client
    // write(client_sock , client_message , strlen(client_message));
    std::cout << client_message << std::endl;
    std::map<std::string, std::string> headers = parseHttpHeaders(client_message, read_size);
    std::cout << headers["Sec-WebSocket-Key"] + WSGUID << std::endl;

    unsigned char *hashArr;
    hashArr = (unsigned char*)malloc(20);
    memset(hashArr, 0, sizeof(unsigned char) * 20);

    // Misc::sha1( headers["Sec-WebSocket-Key"] + WSGUID, hashArr );
    Misc::sha1("dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CAC5AB0DC85B11", hashArr);
    std::cout << hashArr << std::endl;
    
    // std::string hash = "";
    std::string hash = Misc::base64_encode( hashArr, 20 );
    std::cout << hash << std::endl;
    std::string res = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + hash + "\r\n\r\n";
    write(client_sock, res.c_str(), strlen(res.c_str()));
    memset( &client_message, 0, sizeof(client_message));
    // close(client_sock);
  }

  if(read_size == 0)
  {
    puts("Client disconnected");
    fflush(stdout);
  }
  else if(read_size == -1)
  {
    perror("recv failed");
  }
}

std::map<std::string, std::string> WebSocket::parseHttpHeaders(char *msg, int size) {
  std::map<std::string, std::string> header;

  std::string key = "";
  std::string val = "";

  int count = 0;

  // get the method
  // we dont actually care
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

  for (; count < size; count++) {
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

  Misc::PrintMap(header);
  return header;

  // std::cout << "MSG: " << std::endl
  //           << msg << std::endl
  //           << "Count: " << size << std::endl;
}
