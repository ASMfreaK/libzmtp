/*  =========================================================================
    zmtp_tcp_endpoint - TCP endpoint class

    Copyright (c) contributors as noted in the AUTHORS file.
    This file is part of libzmtp, the C ZMTP stack.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#include "zmtp_classes.h"
#ifdef __cplusplus
extern "C" {
#endif
struct zmtp_tcp_endpoint {
    zmtp_endpoint_t base;
    char *addrinfo;
    uint16_t port;
};


zmtp_tcp_endpoint_t *
zmtp_tcp_endpoint_new (const char *ip_addr, unsigned short port)
{
    zmtp_tcp_endpoint_t *self =
        (zmtp_tcp_endpoint_t *) zmalloc (sizeof *self);
    if (!self)
        return NULL;

    //  Initialize base class
    self->base.connect = (int (*) (zmtp_endpoint_t *)) zmtp_tcp_endpoint_connect;
    self->base.listen = (int (*) (zmtp_endpoint_t *)) zmtp_tcp_endpoint_listen;
    self->base.destroy = (void (*) (zmtp_endpoint_t **)) zmtp_tcp_endpoint_destroy;
    // host resolving does not work
    // FIXME: use DNSClient from Dns.h to get addr
    // ip_addr is held in memory of callee,
    // we don't need to hold of it on avr.
    // IMPORTANT: ip_addr is a four-byte representaion of an ip,
    //            it is NOT a string like "192.168.1.1"
    self->addrinfo = ip_addr;
    self->port = port;
    return self;
}


void
zmtp_tcp_endpoint_destroy (zmtp_tcp_endpoint_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zmtp_tcp_endpoint_t *self = *self_p;
        free (self);
        *self_p = NULL;
    }
}


int
zmtp_tcp_endpoint_connect (zmtp_tcp_endpoint_t *self)
{
    assert (self);
    zmtp_tcp_endpoint_t *self_p = (zmtp_tcp_endpoint_t*) self;
    const SOCKET s = get_sock_num();
    if (s == -1)
        return -1;
    socket(s, SnMR::TCP, 0, 0);

    const int rc = connect (
      s, self_p->addrinfo, self_p->port);
    if (rc == 0) {
        close (s);
        return -1;
    }

    return s;
}

int16_t
zmtp_tcp_endpoint_listen (zmtp_tcp_endpoint_t *self)
{
    assert (self);
    zmtp_tcp_endpoint_t *self_p = (zmtp_tcp_endpoint_t*) self;
    /*const int s = socket (AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        return -1;*/
    const SOCKET s = get_sock_num();
    if (s == -1)
        return -1;
    socket(s, SnMR::TCP, self_p->port, 0);
    // can't set sock options on WS5100
    /*const int flag = 1;
    int rc = setsockopt (s, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof flag);
    assert (rc == 0);*/

    // no bind on WS5100
    /*rc = bind (
        s, self->addrinfo, self->addrinfo->ai_addrlen);
    if (rc == 0) {*/
    uint8_t rc = listen(s); //WS5100 only listens to one at a time
    if (rc == 1){
        return s;
    }
    close(s);
    return -1;
    // rc = accept (s, NULL, NULL); //WS5100 no accept
}
#ifdef __cplusplus
}
#endif
