/*
 * This is an implementation of a PUB ZMQ socket on WS5100 shield.
 * It is working on one "socket" of WS5100 - it can operate on
 * only one SUB client at a time. This implementation work quite
 * fast. Here is a simple python script to test it:
 *
 * import zmq
 * import time
 * import sys
 * import bson
 * c = zmq.Context()
 * s = c.socket(zmq.SUB)
 * try:
 *     ip = sys.argv[1]
 * except IndexError:
 *     ip = '192.168.1.1'
 * s.connect("tcp://{}:12000".format(ip))
 * s.setsockopt(zmq.SUBSCRIBE, b'')
 * times = []
 * pt = time.time()
 * try:
 *     while True:
 *        m = s.recv()
 *        t = time.time()
 *        times.append(t-pt)
 *        pt = t
 * except:
 *     print("gracefully stopping")
 *     s.setsockopt(zmq.UNSUBSCRIBE, b'')
 *     s.close()
 *     at = sum(times)/len(times)
 *     mt = sum([(at - x)**2 for x in times]) / len(times)
 *     print ("min/avg/max/mdev =", min(times), at, max(times), mt)
 *
 * The script outputs following string, all values are in seconds:
 * min/avg/max/mdev = 1.9073486328125e-06 0.0009596273221377656 0.008733987808227539 7.893759203847338e-08
 */
#include <bson.h>
#include <zmtpduino.h>
#include <zmtp_msg.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 1);
IPAddress myDns(192,168,1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


zmtp_channel_t *d;

void setup() {
  int rc;
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  Serial.println("Ethernet init");
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  Serial.println("initd'. begin connecting:");
  // char endpoint_str[] = "tcp://192.168.1.2"
  // uint8_t ip[] = {192, 168, 1, 9};
}

void loop() {
  zmtp_channel_t * d = zmtp_channel_new ();
  if (!d)
    return;
  zmtp_metadata_property_t st = {
    .name_len=11,
    .name="Socket-Type",
    .value_len=3,
    .value="PUB"
  };
  zmtp_metadata_property_t* props[] = {&st};
  zmtp_metadata_t meta = {
    .properties=props,
    .properties_count=1
  };
  uint8_t ip[] = {192,168, 1, 1};
  while(zmtp_channel_tcp_listen(d, ip, 12000, &meta) != 0){
    Serial.println("nyaro~n listening");
  }
  
  //if(rc != 0){Serial.println("nyaro~n connecting"); return;}
  Serial.println("got a listener, yay!");
  Serial.println("sock status");
  int32_t va0 = digitalRead(A0);
  char message[] =  {BSON_DOCUMENT(1, INT32, a0, 2, va0)};
  int vi = ((int[]){ BS_EVAL( BS_VERIND(1, INT32, a0, 2) ) })[0];
  //char message[] =  "DEADBEEFFEEDMEPLZ";
  zmtp_msg_t *msg = zmtp_msg_from_const_data(0, message, sizeof message);
  while(W5100.readSnSR(0) !=SnSR::CLOSE_WAIT){
    ((sint32_t*)(message+vi))->value = digitalRead(A0);
    if(zmtp_channel_send(d, msg)==-1){Serial.println("nyaro~n sending"); return;}
  }
  zmtp_channel_destroy(&d);
  zmtp_msg_destroy(&msg);
}
