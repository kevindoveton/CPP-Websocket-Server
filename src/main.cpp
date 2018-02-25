// #include "main.h"

#include <string>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <sys/socket.h>
#include <iostream>

#include <cryptlite/sha1.h>
#include <openssl/sha.h>

int main(int argc, char *argv[]) {
  // WebSocket websocket(3000);
  // unsigned char hash[20];
  
  // std::string sha1base64 = cryptlite::sha1::hash_base64("");
  // std::cout << sha1base64 << std::endl;
  const char ibuf[] = "dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CAC5AB0DC85B11";
  unsigned char obuf[20];

  SHA_CTX sctx;
  (void)SHA1_Init(&sctx);
	(void)SHA1_Update(&sctx, ibuf, strlen(ibuf));
	(void)SHA1_Final(obuf, &sctx);

  for (int i = 0; i < 20; i++) {
    std::cout << obuf[i];
  }
  printf("\n");

  return 0;
}
