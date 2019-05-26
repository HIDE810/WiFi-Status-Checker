#include "sockets.h"

s32 sock = -1, csock = -1;

void IP(const char *fmt, ...)
{
	if(sock>0) close(sock);
	if(csock>0) close(csock);
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}