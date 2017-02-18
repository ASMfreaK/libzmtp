#ifndef __ZMTP_ARDUINO_TYPES_H_
#define __ZMTP_ARDUINO_TYPES_H_
#if defined(__AVR__)
#include <utility/w5100.h>
#include <utility/socket.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int16_t ssize_t;
int16_t arduino_get_sock_num();
int accept(SOCKET s);


#ifdef __cplusplus
}
#endif
#endif
#endif
