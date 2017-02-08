#include "zmtpduino.h"

#include "src/zmtp_endpoint.c"
#include "src/zmtp_msg.c"
#include "src/zmtp_classes.h"

#if defined(arduinotcp)
#   include "src/zmtp_tcp_arduino_endpoint.cpp"
#endif
#if defined(hasnormaltcp)
#   include "src/zmtp_tcp_endpoint.c"
#endif

//#include "src/zmtp_channel.c"

//#include "src/zmtp_dealer.c"
