#include <stdarg.h>
#include <3ds.h>

#ifndef SOCKETS_H
#define SOCKETS_H

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

int ret;
struct sockaddr_in server;
static u32 *SOC_buffer = NULL;
void IP(const char *fmt, ...);

#endif