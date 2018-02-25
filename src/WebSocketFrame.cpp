#include "WebSocketFrame.hpp"

WebSocketFrame::WebSocketFrame() {
  reset();
  return;
}

WebSocketFrame::WebSocketFrame(std::string f) {
  reset();
  readFrame(f);
  return;
}

bool WebSocketFrame::readFrame(std::string f) {
  if (f.length() < 1) {
    return false; // TODO: make this right
  }

  // byte 1 - length and opcode
  std::bitset<8> bOne = std::bitset<8>(f.c_str()[0]);
  _fin = bOne.test(0);
  _mode = bOne.to_ulong() & 0b00001111; // NOTE: bits 4 and 5 are not ever used.. but part of the spec

  // byte 2 - mask and payload length
  std::bitset<8> bTwo = std::bitset<8>(f.c_str()[1]);
  _maskBit = bTwo.test(7);
  _payloadLength = bTwo.to_ulong() & 0b01111111;
  unsigned int payloadExtraBytes = 0;

  if (_payloadLength == 126) {
    payloadExtraBytes = 2;
  } else if (_payloadLength == 127) {
    payloadExtraBytes = 4;
  }

  // add the extra payload bytes to length
  for (unsigned int i = 0; i < payloadExtraBytes; i++) {
    std::bitset<8> bits = std::bitset<8>( f.c_str()[2 + i] );
    _payloadLength += bits.to_ulong();
  }

  // byte 3(ish) - get the mask
  for (unsigned int i = 0; i < 4; i++) {
    std::bitset<8> maskSection = std::bitset<8>( f.c_str()[3 + payloadExtraBytes + i] );
    _mask[i] = maskSection;
  }

  for (unsigned int i = 0; i < _payloadLength; i++) {
    const char c = f.c_str()[i + 7 + payloadExtraBytes] ^ _mask[i % 4].to_ulong();
    std::cout << c;
  }
  std::cout << std::endl;

  return true;
}

void WebSocketFrame::reset() {
  // _mask.reset();
}
