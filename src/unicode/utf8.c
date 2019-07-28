#include <stddef.h>

static int
utf8_decode_t1(int *x, const char *s, int n)
{
	int i;

	for (i = 1; i < n; i++) {
		if ((*s & 0xc0) != 0x80)
			return -1;
		*x <<= 6;
		*x |= *s++ & 0x7f;
	}

	return n;
}

int
utf8_decode(int *x, const char *s)
{
	if ((*s & 0x80) == 0) {
		*x = *s & 0x7f;
		return 1;
	}

	if ((*s & 0xe0) == 0xc0) {
		*x = *s & 0x1f;
		return utf8_decode_t1(x, s + 1, 2);
	}

	if ((*s & 0xf0) == 0xe0) {
		*x = *s & 0x0f;
		return utf8_decode_t1(x, s + 1, 3);
	}

	if ((*s & 0xf1) == 0xf0) {
		*x = *s & 0x07;
		return utf8_decode_t1(x, s + 1, 4);
	}

	return -1;
}
