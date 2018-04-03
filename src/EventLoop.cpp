//
// Created by Kevin Doveton on 12/03/2018.
//

#include "EventLoop.hpp"

EventLoop::EventLoop() {
  _maxEvents = EPOLL_MAX_EVENTS;
  #if __linux__
    _timeout = -1; // block indefinitely
    _epfd = epoll_create1(0);
    _call = 0;
  #endif
  #if __APPLE__
    _epfd = kqueue();
  #endif

  std::cout << "Epoll: " << _epfd << std::endl;
}

EventLoop::~EventLoop() {
  #if __linux__
  #endif
  #if __APPLE__
    close(_epfd);
  #endif
}

void EventLoop::Run() {
  #if __APPLE__
  #define KQUEUE_TIMEOUT (125 * 1000000)
    struct timespec timeout;
    timeout.tv_nsec = KQUEUE_TIMEOUT; // .125s / 125ms
    timeout.tv_sec = 0;
  #endif

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
    int event_count = kevent(_epfd, events_to_monitor, NUM_EVENT_SLOTS, event_data, num_files, &timeout);
    if ((event_count < 0) || (event_data[0].flags == EV_ERROR)) {
      /* An error occurred. */
      fprintf(stderr, "An error occurred (event count %d).  The error was %s.\n", event_count, strerror(errno));
      break;
    }
    if (event_count) {
      printf("Event %" PRIdPTR " occurred.  Filter %d, flags %d, filter flags %s, filter data %" PRIdPTR ", path %s\n",
          event_data[0].ident,
          event_data[0].filter,
          event_data[0].flags,
          flagstring(event_data[0].fflags),
          event_data[0].data,
          (char *)event_data[0].udata);
    } else {
      printf("No event.\n");
    }

    /* Reset the timeout.  In case of a signal interrruption, the
       values may change. */
    timeout.tv_sec = 0;
    timeout.tv_nsec = KQUEUE_TIMEOUT;
    #endif
  }

}

void EventLoop::AddHandler(int fd, Handler *handler, unsigned int events) {
  _handlers[fd] = handler;

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
    unsigned int vnode_events;
    vnode_events = NOTE_DELETE |  NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE;

    void *user_data;
    /* Set up a list of events to monitor. */
    EV_SET( &events_to_monitor[0], fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, vnode_events, 0, user_data);
  #endif
}

void EventLoop::RemoveHandler(int fd) {

}
