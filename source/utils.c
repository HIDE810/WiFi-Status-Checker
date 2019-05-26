#include "utils.h"

void Utils_U8_To_U16(u16 *buf, const u8 *input, size_t bufsize) {
	ssize_t units = utf8_to_utf16(buf, input, bufsize);

	if (units < 0)
		units = 0;

	buf[units] = 0;
}