#include <stdint.h>

#include "buf.h"
#include "textfmt.h"


void
textfmt_int(Buf *b, int flags, int width, int prec, int base, intmax_t v)
{
	if (v < 0) {
		buf_pushc(b, '-');
		v = -v;
	}

	textfmt_uint(b, flags, width, prec, base, v);
}

void
textfmt_uint(Buf *b, int flags, int width, int prec, int base, uintmax_t v)
{
	const char *text = "0123456789abcdef";
	int start;

	(void)flags;
	(void)width;
	(void)prec;

	start = b->len;
	do {
		buf_pushc(b, text[v % base]);
	} while ((v /= base) != 0);

	buf_reverse(b, start, b->len - 1);
}
