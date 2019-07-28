#include <stddef.h>

int
sjis_decode(int *x, const char *s)
{
	int c;

	c = *s & 0xff;
	if ((c >= 0x00 && c <= 0x7f) || (c >= 0xa1 && c <= 0xdf)) {
		*x = c;
		return 1;
	}

	if ((c >= 0x81 && c <= 0x9f) || (c >= 0xe0 && c <= 0xef)) {
		*x = c;
		c = s[1] & 0xff;
		if ((c >= 0x40 && c <= 0x7e) || (c >= 0x80 && c <= 0xfc)) {
			*x <<= 8;
			*x |= c;
			return 2;
		}
	}

	return -1;
}
