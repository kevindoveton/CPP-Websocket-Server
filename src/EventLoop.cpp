//
// Created by Kevin Doveton on 12/03/2018.
//

#include "EventLoop.hpp"

EventLoop::EventLoop() {
  _maxEvents = 10;
  #if __linux__
    _timeout = -1; // block indefinitely
    _epfd = epoll_create1(0);
    _call = 0;
  #endif
  #if __APPLE__
    _epfd = kqueue();
    _currentEventNum = 0;
  #endif

  std::cout << "Epoll: " << _epfd << std::endl;
}

EventLoop::~EventLoop() {
  #if __linux__
  #endif
  #if __APPLE__
    free(_evSet);
    free(_evList);
    close(_epfd);
  #endif
}

void EventLoop::Run() {
  while (true) {
    #if __linux__
      struct epoll_event events[EPOLL_MAX_EVENTS];
      int nfds = epoll_wait(_epfd, events, _maxEvents, _timeout /* Timeout */);
      for (int i = 0; i < nfds; ++i) {
        int fd = events[i].data.fd;
        Handler *h = _handlers[fd];
        h->Handle(events[i]);
      }
    #endif
    #if __APPLE__
    _nfds = kevent(_epfd, NULL, 0, _evSet, _currentEventNum, NULL);
    if ((_nfds < 0)) {
      /* An error occurred. */
      fprintf(stderr, "An error occurred (event count %d).  The error was %s.\n", _nfds, strerror(errno));
//      break;
    }
    if (_nfds) {
      for (int i = 0; i < _nfds; i++) {
        std::cout << "connection" << std::endl;
          Handle_Event_t ev;
          ev.data.fd = _evSet[i].ident;
          _handlers[ev.data.fd]->Handle(ev);
      }
//      printf("Event %" PRIdPTR " occurred.  Filter %d, flags %d, filter flags %s, filter data %" PRIdPTR ", path %s\n",
//          event_data[0].ident,
//          event_data[0].filter,
//          event_data[0].flags,
//          flagstring(event_data[0].fflags),
//          event_data[0].data,
//          (char *)event_data[0].udata);
    }

    #endif
  }

}

void EventLoop::AddHandler(int fd, Handler *handler, unsigned int events) {
  _handlers[fd] = handler;
  std::cout << "fd " << fd << std::endl;
  #if __linux__
    struct epoll_event e = epoll_event{};
    e.data.fd = fd;
    e.events = events;

    if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &e) < 0) {
      std::cerr << "Failed to insert handler to epoll" << std::endl;
    }

    std::cout << "Added Handler" << std::endl;
  #endif
  #if __APPLE__
//    int *tmp = (int*)malloc(sizeof(int));
//    *tmp = fd;
//    void *user_data = tmp;
    /* Set up a list of events to monitor. */
    EV_SET( &_evSet[_currentEventNum], fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
    kevent(_epfd, &_evSet[_currentEventNum], 1, NULL, 0, NULL);
    _currentEventNum++;
  #endif
}

void EventLoop::RemoveHandler(int fd) {

}

void EventLoop::increaseEventCount() {
  _maxEvents = (_maxEvents * 2) + 1;
  _evList = (struct kevent*)malloc(_maxEvents * sizeof(struct kevent));
  struct kevent *newEvSet = (struct kevent*)realloc(_evSet, (_maxEvents * sizeof(struct kevent)));
  _evSet = newEvSet;
}
