//
// Created by Kevin Doveton on 12/03/2018.
//

#ifndef CPP_WEB_SOCKET_EVENTLOOP_HPP
#define CPP_WEB_SOCKET_EVENTLOOP_HPP

#include <sys/epoll.h>
#include <iostream>
#include <map>
#include "Handler.hpp"

#define EPOLL_MAX_EVENTS 10

class Handler;
class WebSocket;

class EventLoop {
  public:
    EventLoop();
    void Run();
    void AddHandler(int fd, Handler *handler, unsigned int events);
    void RemoveHandler(int fd);
  protected:
    int _epfd;
    int _nfds;
    int _call;
    int _maxEvents;
    int _timeout;
    std::map<int, Handler* > _handlers;
};


#endif //CPP_WEB_SOCKET_EVENTLOOP_HPP
