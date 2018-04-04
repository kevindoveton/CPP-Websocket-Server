//
// Created by Kevin Doveton on 12/03/2018.
//

#ifndef CPP_WEB_SOCKET_EVENTLOOP_HPP
#define CPP_WEB_SOCKET_EVENTLOOP_HPP

#if __linux__
  #include <sys/epoll.h>
#endif
#if __APPLE__
  #include <sys/types.h>
  #include <sys/event.h>
  #include <sys/time.h>
  #include <fcntl.h>
#endif

#include <unistd.h>
#include <iostream>
#include <map>
#include <cinttypes>

#include "Handler.hpp"

class Handler;

class EventLoop {
  public:
    EventLoop();
    ~EventLoop();
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

    #if __APPLE__
      void increaseEventCount();
      struct kevent *_evSet;
      struct kevent *_evList;
      int _currentEventNum;
    #endif
};


#endif //CPP_WEB_SOCKET_EVENTLOOP_HPP
