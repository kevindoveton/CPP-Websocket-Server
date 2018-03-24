//
// Created by Kevin Doveton on 10/03/2018.
//

#include "WebSocketConnection.hpp"

WebSocketConnection::WebSocketConnection(struct sockaddr_in client, int clientFd) {
  _accepted = false;
  _client = client;
  _clientFd = clientFd;
  _state = STATE::UNKNOWN;

  AcceptConnection();
}

WebSocketConnection::~WebSocketConnection() {
  CloseConnection();
}

void WebSocketConnection::CloseConnection() {
  // Set up close frame
  std::map<std::string, std::string> m;
  m["FIN"] = "1";
  m["OPCODE"] = std::to_string(WebSocketFrame::CLOSE);
  m["PAYLOAD_LENGTH"] = "0";

  SendFrame(WebSocketFrame(m));
  _state = STATE::CLOSING;

  // TODO: wait for the client to respond to us

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
  shutdown(_clientFd, SHUT_WR); // shut read and write
  while (recv(_clientFd, nullptr, 1, 0) > 0); // get rid of all data - we don't really care any more
  close(_clientFd); // close the connection
  _state = STATE::CLOSED;
}

void WebSocketConnection::AcceptConnection() {
  if (_accepted) { return; }

  std::cout << "Accept" << std::endl;
  ssize_t read_size;
  char client_message[MAX_FRAME_SIZE];
  while ((read_size = read(_clientFd, client_message , sizeof(client_message))) < 0) {
    std::cout << "waiting" << std::endl;
  }
  std::cout << read_size << std::endl;
  // get the http headers
  std::map<std::string, std::string> headers = Http::ParseHttpHeaders(client_message, read_size);

  // generate the sec-websocket-accept key
  std::string key = headers[ "Sec-WebSocket-Key" ];
  key += WSGUID;

  std::string hash = cryptlite::sha1::hash_base64(key);

  // send a http response
  std::string res = "HTTP/1.1 101 Switching Protocols\r\n"
                    "Upgrade: websocket\r\n"
                    "Connection: Upgrade\r\n"
                    "Sec-WebSocket-Accept: " + hash + "\r\n"
                    "\r\n";

  write(_clientFd, res.c_str(), strlen(res.c_str()));

  // mark the client as accepted
  _accepted = true;
  _state = STATE::OPEN;

  std::cout << "Accepted Connection" << std::endl;

}

std::string WebSocketConnection::GetMessage() {
  WebSocketFrame f = GetFrame();
  return f.GetPayload();
}

void WebSocketConnection::SendMessage(std::string msg) {

}

WebSocketFrame WebSocketConnection::GetFrame() {
  ssize_t read_size;
  char client_message[MAX_FRAME_SIZE];
  while ((read_size = read(_clientFd, client_message, sizeof(client_message))) < 0) { /* busy wait */ }
  std::string frame = client_message;

  return WebSocketFrame(frame);
}
