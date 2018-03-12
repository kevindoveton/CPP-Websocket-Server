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

class EventLoop {
  public:
    EventLoop();
    void Run();
    void AddHandler(int fd, Handler *handler, unsigned int events);

  protected:
    int _epfd;
    int _nfds;
    struct epoll_event _events[EPOLL_MAX_EVENTS];
    int _maxEvents;
    int _timeout;
    std::map<int, Handler* > _handlers;
};


#endif //CPP_WEB_SOCKET_EVENTLOOP_HPP
