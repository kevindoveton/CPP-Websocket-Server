//
// Created by Kevin Doveton on 10/03/2018.
//

#include "WebSocketFrame.hpp"

WebSocketFrame::WebSocketFrame() {
  Reset();
}

WebSocketFrame::WebSocketFrame(std::string f) {
  Reset();
  ReadFrame(f);
}

bool WebSocketFrame::ReadFrame(std::string f) {
  if (f.length() < 1) {
    return false; // TODO: make this right
  }

//  for (int i = 0; i < f.length(); i++) {
//    std::cout << std::bitset<8>(f.c_str()[i]) << std::endl;
//  }

  // byte 1 - length and opcode
  std::bitset<8> bOne = std::bitset<8>(f.c_str()[0]);
  _fin = bOne.test(0);
  _opcode = bOne.to_ulong() & 0b00001111; // NOTE: bits 4 and 5 are not ever used.. but part of the spec
//  std::cout << _opcode << std::endl;
  // byte 2 - mask and payload length
  std::bitset<8> bTwo = std::bitset<8>(f.c_str()[1]);
  _maskBit = bTwo.test(7);
  _payloadLength = bTwo.to_ulong() & 0b01111111;
  unsigned int payloadExtraBytes = 0;

  if (_payloadLength == 126) {
    payloadExtraBytes = 2;
    // std::cout << "126..." << std::endl;
  } else if (_payloadLength == 127) {
    payloadExtraBytes = 4;
    // std::cout << "127..." << std::endl;
  }

  // add the extra payload bytes to length
  for (unsigned int i = 0; i < payloadExtraBytes; i++) {
    std::bitset<8> bits = std::bitset<8>( f.c_str()[2 + i] );
    // std::cout << 2 + i << std::endl;
    _payloadLength += bits.to_ulong();
  }

  // std::cout << _payloadLength << std::endl;

  // byte 3(ish) - get the mask
  for (unsigned int i = 0; i < 4; i++) {
    std::bitset<8> maskSection = std::bitset<8>( f.c_str()[2 + payloadExtraBytes + i] );
    _mask[i] = maskSection;
    // std::cout << maskSection.to_ulong() << std::endl;
  }


  std::stringstream payloadData;
  for (unsigned int i = 0; i < _payloadLength; i++) {
    const char c = f.c_str()[i + 6 + payloadExtraBytes] ^ _mask[i % 4].to_ulong();
    payloadData << c;
  }

  _payloadData = payloadData.str();

  return true;
}

WebSocketFrame::WebSocketFrame(std::map<std::string, std::string> f) {
  std::map<std::string, std::string>::iterator it;
  Reset();

  it = f.find("FIN");
  if (it != f.end()) {
    _fin = Utils::StringToBool(f["FIN"]);
  }

  it = f.find("MASK");
  if (it != f.end()) {
    _maskBit = Utils::StringToBool(f["MASK"]);
  }

  it = f.find("OPCODE");
  if (it != f.end()) {
    _opcode = std::stoi(f["OPCODE"]);
  }

  it = f.find("PAYLOAD_LENGTH");
  if (it != f.end()) {
    _payloadLength = std::stoul(f["PAYLOAD_LENGTH"]);
  }

  it = f.find("PAYLOAD_DATA");
  if (it != f.end()) {
    _payloadData = f["PAYLOAD_DATA"];
  }

  it = f.find("MASK_KEY");
  if (it != f.end()) {
    char *m = new char[4];
    std::strcpy(m, f["MASK_KEY"].c_str());

    for (int i = 0; i < 4; i++) {
      _mask[i] = std::bitset<8>(m[i]);
    }
  }
}

std::string WebSocketFrame::GetFrame() {
  std::string ret;
  std::stringstream f;
  f << _fin
    << "000"
    << std::bitset<4>(_opcode)
    << _maskBit
    << std::bitset<7>(_payloadLength);

  std::bitset<16> bs;
  while (f >> bs) {
    ret += static_cast<char>(bs.to_ulong());
  }

//  std::cout << "frame: " << std::endl << f.str() << std::endl;

//  return f.str();
  return ret;
}

void WebSocketFrame::Reset() {
  _fin = true;
  _opcode = 1;
  _maskBit = false;
  _payloadData = "";
  _payloadLength = 0;
}

