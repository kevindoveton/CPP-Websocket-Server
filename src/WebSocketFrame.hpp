//
// Created by Kevin Doveton on 10/03/2018.
//

#ifndef CPP_WEB_SOCKET_WEBSOCKETFRAME_HPP
#define CPP_WEB_SOCKET_WEBSOCKETFRAME_HPP

#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <cstring>
#include <sstream>

#include "Utils.hpp"

class WebSocketFrame {
  public:
    WebSocketFrame();
    explicit WebSocketFrame(std::string);
    explicit WebSocketFrame(std::map<std::string, std::string> f);

    bool ReadFrame(std::string f);
    std::string GetFrame();

    void Reset();

    // Sec 5.2, Page 29 Opcode
    enum { CONTINUATION = 0x0, TEXT = 0x1, BINARY = 0x2,
      // 0x3 - 0x7 are reserved
      CLOSE = 0x8, PING = 0x9, PONG = 0xA
      // 0xB - 0xF are reserved
    };

  protected:
    bool _fin;
    unsigned long _opcode;
    bool _maskBit;
    unsigned long _payloadLength;
    std::string _payloadData;
    std::bitset<8> _mask[4]; // 32 bits
};

/*
Frame format:
​​
 0               \   1           \       2       \           3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
*/

#endif //CPP_WEB_SOCKET_WEBSOCKETFRAME_HPP
