//
// Created by Kevin Doveton on 10/03/2018.
//

#include "WebSocketConnection.hpp"

WebSocketConnection::WebSocketConnection(struct sockaddr_in client, int clientFd) {
  _accepted = false;
  _client = client;
  _clientFd = clientFd;

  manage();
}

WebSocketConnection::~WebSocketConnection() {
  CloseConnection();
}

void WebSocketConnection::manage() {
  char client_message[2500];
  ssize_t read_size;
  while( (read_size = read(_clientFd, client_message , sizeof(client_message))) > 0 ) {
    if (!_accepted) {

      // get the http headers
      std::map<std::string, std::string> headers = Http::ParseHttpHeaders(client_message, read_size);

      // generate the sec-websocket-accept key
      std::string key = headers[ "Sec-WebSocket-Key" ];
      key += WSGUID;

      std::string hash = cryptlite::sha1::hash_base64(key);

      // send a http response
      std::string res = "HTTP/1.1 101 Switching Protocols\r\n"
                        "Upgrade: websocket\r\n"
                        "Connection: Upgrade\r\nSec-WebSocket-Accept: " + hash + "\r\n\r\n";
      write(_clientFd, res.c_str(), strlen(res.c_str()));

      // mark the client as accepted
      _accepted = true;

    } else {

      // client has already been accepted
      std::cout << "Reading Frame" << std::endl;
      std::string frame = client_message;
      WebSocketFrame f = WebSocketFrame(frame);
      std::cout << "End of frame" << std::endl;
    }

    memset(&client_message, 0, sizeof(client_message));
  }
}

void WebSocketConnection::CloseConnection() {
  // Set up close frame
  std::map<std::string, std::string> m;
  m["FIN"] = "1";
  m["OPCODE"] = std::to_string(WebSocketFrame::CLOSE);
  m["PAYLOAD_LENGTH"] = "0";

  SendFrame(WebSocketFrame(m));
  closeSocket();
}

void WebSocketConnection::SendFrame(WebSocketFrame f) {
  // get the frame as a string
  std::string frame = f.GetFrame();

  // send it
  write(_clientFd, frame.c_str(), frame.length());
}

void WebSocketConnection::closeSocket() {
  // shut the connection
  shutdown(_clientFd, SHUT_WR);
  while (recv(_clientFd, nullptr, 1, 0) > 0);
  close(_clientFd);
}

