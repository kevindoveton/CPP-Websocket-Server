//
// Created by Kevin Doveton on 12/03/2018.
//

#include "WebSocketEvents.hpp"

void WebSocketEvents::onMsg(HandlerEvent_t e) {
  std::cout << e.wsFrame.GetPayload() << std::endl;
}
