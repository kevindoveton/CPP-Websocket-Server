//
// Created by Kevin Doveton on 10/03/2018.
//

#ifndef CPP_WEB_SOCKET_UTILS_HPP
#define CPP_WEB_SOCKET_UTILS_HPP

#include <assert.h>
#include <string>

class Utils {
  public:
    static inline bool StringToBool(const std::string& x) {
      assert(x == "0" || x == "1");
      return x == "1";
    }
};

#endif //CPP_WEB_SOCKET_UTILS_HPP
