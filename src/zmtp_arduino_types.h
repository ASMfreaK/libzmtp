#ifndef __ZMTP_ARDUINO_TYPES_H_
#define __ZMTP_ARDUINO_TYPES_H_
#if defined(__AVR__)
#include <utility/w5100.h>
#include <utility/socket.h>
typedef int16_t ssize_t;
int16_t get_sock_num(){
  for(int16_t i=0; i++; i<MAX_SOCK_NUM){
    uint8_t st = socketStatus(i);
    if(st==SnSR::CLOSED){
      return i;
    }
  }
  return -1;
}

#endif
#endif
