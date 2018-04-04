//
// Created by Kevin Doveton on 11/03/2018.
//

#ifndef CPP_WEB_SOCKET_WEBSOCKET_HPP
#define CPP_WEB_SOCKET_WEBSOCKET_HPP


#include <cstdint>
#include <map>
#include "Http.hpp"
#include "Handler.hpp"
#include "WebSocketConnection.hpp"
#include "WebSocketFrame.hpp"

class WebSocketFrame;
class WebSocketConnection;
class Handler;
class WebSocket;

struct HandlerEvent_t {
    Handle_Event_t handleEvent;
    WebSocketConnection *wsConnection;
    WebSocket *ws;
    WebSocketFrame wsFrame;
};

typedef void(*WS_CB_FUNC)(struct HandlerEvent_t frame);

class WebSocket : public Handler {
  public:
    /*
     * WebSocket
     * @param {uint16_t} port - the port in which to start the server
    */
    explicit WebSocket(uint16_t port);

    /*
     * ~WebSocket
     * Close all web socket connections and delete from memory
    */
    ~WebSocket();

    /*
     * On
     * Event handler for messages
     * @param {int} e - the event to handle
     * @param {WS_CB_FUNC} f - the callback function
    */
    void On(int e, WS_CB_FUNC);

    /*
     * Broadcast
     * Send a message to all connected clients
     * @param {std::string} msg - the message to send
    */
    void Broadcast(std::string msg);

    /*
     * AddClient
     * Add an already accepted connection to the managed connections
     * @param {strunt sockaddr_in} client - client sock info
     * @param {int} client_sock - client file descriptor
    */
    void AddClient( struct sockaddr_in client, int client_sock);

    /*
     * Handle
     * Handle the epoll callback
     * @param {epoll_event} e - the epoll event
     * @return TODO: what should this return
    */
    int Handle(Handle_Event_t e) override;

    /*
     * Events enum
     * Contains all valid and supported events
    */
    enum { OPEN, CLOSE, MESSAGE };

    void isInit() { std::cout << "ini" << _init << std::endl; }
  protected:
    /*
     * _eventFunc
     * Contains all callback functions
     * int - the event to handle
     * WS_CB_FUNC - the callback function
    */
    std::map <int, WS_CB_FUNC> _eventFunc;

    /*
     * _connections
     * Map containing all active web socket connections
     * int - file descriptor of connection
     * WebSocketConnection* - pointer to the connection object
    */
    std::map<int, WebSocketConnection*> _connections;

    int _init = 0;
};


#endif //CPP_WEB_SOCKET_WEBSOCKET_HPP
