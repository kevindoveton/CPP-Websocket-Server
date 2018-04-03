//
// Created by Kevin Doveton on 12/03/2018.
//

#ifndef CPP_WEB_SOCKET_HANDLER_HPP
#define CPP_WEB_SOCKET_HANDLER_HPP

#if __linux__
#include <sys/epoll.h>
#endif
#if __APPLE__
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif
#include <fcntl.h>
#include <iostream>
#include "EventLoop.hpp"


class EventLoop;

#if __linux__
typedef epoll_event Handle_Event_t;
#endif
#if __APPLE__
union Handle_Event_Data_t {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
};

struct Handle_Event_t {
    uint32_t     events;    /* Epoll events */
    Handle_Event_Data_t data;      /* User data variable */
};
#define EPOLLIN 0
#define EPOLLHUP 0
#endif

class Handler {
  public:
    virtual int Handle(Handle_Event_t e) = 0;
    void inline SetNonBlocking(int fd) {
      #if __linux__
      _flags = fcntl(fd, F_GETFL, 0); fcntl(fd, F_SETFL, _flags | O_NONBLOCK);
      #endif
    }

    void inline SetEventLoopInstance(EventLoop *ev) { _ev = ev; }

  protected:
    EventLoop *_ev;
    int _flags;

};


#endif //CPP_WEB_SOCKET_HANDLER_HPP
