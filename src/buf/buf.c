#include <stddef.h>

#include "buf.h"


void
buf_init(Buf *b, size_t len, size_t cap, void *data)
{
	b->len = len;
	b->cap = cap;
	b->data = data;
}

int
buf_push_u8(Buf *b, int x)
{
	if (b->len >= b->cap)
		return -1;

	b->data[b->len++] = x;

	return 0;
}

void
buf_reverse(Buf *b, int start, int end)
{
	char *s = b->data + start;
	char *e = b->data + end;
	char t;

	while (s < e) {
		t = *s;
		*s = *e;
		*e = t;
		s++;
		e--;
	}
}
