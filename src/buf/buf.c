#include <stdlib.h>
#include <string.h>

#include "buf.h"


int
buf_init(Buf *b, size_t cap)
{
	if (cap == 0) {
		b->data = NULL;
	} else {
		if ((b->data = malloc(cap)) == NULL)
			return -1;
	}
	b->len = 0;
	b->cap = cap;

	return 0;
}

void
buf_free(Buf *b)
{
	free(b->data);
}

int
buf_expand(Buf *b, size_t need)
{
	size_t cap;
	void *p;

	cap = b->cap;
	do {
		cap <<= 1;
	} while (cap < need);

	if ((p = realloc(b->data, cap)) == NULL)
		return -1;
	b->data = p;
	b->cap = cap;

	return 0;
}

void
buf_clear(Buf *b)
{
	b->len = 0;
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

int
buf_pushc(Buf *b, int ch)
{
	if (b->len + 1 >= b->cap)
		if (buf_expand(b, b->len + 1) != 0)
			return -1;
	b->data[b->len++] = ch;

	return 0;
}

int
buf_pushstr(Buf *b, const char *s)
{
	return buf_pushoct(b, s, strlen(s));
}

int
buf_pushstrz(Buf *b, const char *s)
{
	return buf_pushoct(b, s, strlen(s) + 1);
}

int
buf_pushoct(Buf *b, const void *oct, size_t len)
{
	if (b->len + len >= b->cap)
		if (buf_expand(b, b->len + len) != 0)
			return -1;

	memcpy(b->data + b->len, oct, len);
	b->len += len;

	return 0;
}
