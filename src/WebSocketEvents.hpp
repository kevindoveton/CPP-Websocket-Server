//
// Created by Kevin Doveton on 12/03/2018.
//

#ifndef CPP_WEB_SOCKET_WEBSOCKETEVENTS_HPP
#define CPP_WEB_SOCKET_WEBSOCKETEVENTS_HPP


#include <iostream>

class WebSocketEvents {
  public:
    static void onMsg() { std::cout << "Got Message" << std::endl; }
};


#endif //CPP_WEB_SOCKET_WEBSOCKETEVENTS_HPP
