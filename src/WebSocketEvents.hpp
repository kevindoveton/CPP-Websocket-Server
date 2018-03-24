//
// Created by Kevin Doveton on 12/03/2018.
//

#ifndef CPP_WEB_SOCKET_WEBSOCKETEVENTS_HPP
#define CPP_WEB_SOCKET_WEBSOCKETEVENTS_HPP


#include <iostream>
#include "WebSocket.hpp"

class WebSocketEvents {
  public:
    static void onMsg(HandlerEvent_t e);
};


#endif //CPP_WEB_SOCKET_WEBSOCKETEVENTS_HPP
