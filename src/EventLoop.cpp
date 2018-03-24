//
// Created by Kevin Doveton on 12/03/2018.
//

#include "EventLoop.hpp"

EventLoop::EventLoop() {
  _maxEvents = EPOLL_MAX_EVENTS;
  _timeout = -1; // block indefinitely
  _epfd = epoll_create1(0);
  _call = 0;
  std::cout << "Epoll: " << _epfd << std::endl;
}

void EventLoop::Run() {

  while (true) {
    struct epoll_event events[EPOLL_MAX_EVENTS];
    int nfds = epoll_wait(_epfd, events, _maxEvents, _timeout /* Timeout */);
    for (int i = 0; i < nfds; ++i) {
      int fd = events[i].data.fd;
      Handler *h = _handlers[fd];
      h->Handle(events[i]);
    }
  }

}

void EventLoop::AddHandler(int fd, Handler *handler, unsigned int events) {

  _handlers[fd] = handler;
  struct epoll_event e = epoll_event{};
  e.data.fd = fd;
  e.events = events;

  if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &e) < 0) {
    std::cerr << "Failed to insert handler to epoll" << std::endl;
  }

  std::cout << "Added Handler" << std::endl;

}

void EventLoop::RemoveHandler(int fd) {

}
