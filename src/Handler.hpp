//
// Created by Kevin Doveton on 12/03/2018.
//

#ifndef CPP_WEB_SOCKET_HANDLER_HPP
#define CPP_WEB_SOCKET_HANDLER_HPP

#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>
#include "EventLoop.hpp"


class EventLoop;

class Handler {
  public:
    virtual int Handle(epoll_event e) = 0;
    void inline SetNonBlocking(int fd) { _flags = fcntl(fd, F_GETFL, 0); fcntl(fd, F_SETFL, _flags | O_NONBLOCK);}
    void inline SetEventLoopInstance(EventLoop *ev) { _ev = ev; }

  protected:
    EventLoop *_ev;
    int _flags;

};


#endif //CPP_WEB_SOCKET_HANDLER_HPP
