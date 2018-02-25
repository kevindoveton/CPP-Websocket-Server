#ifndef KDMISC_H
#define KDMISC_H

#include <map>
#include <iostream>
#include <string>
// #include "SHA1.h"

class Misc {
  public:
    static void PrintMap(std::map<std::string, std::string> map);
    static std::string base64_decode(std::string const& encoded_string);
    static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    static inline bool is_base64(unsigned char c);
    static void sha1(const std::string input, unsigned char *hash);
};

#endif