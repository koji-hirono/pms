#include "str.h"
#include "strreader.h"


void
strreader_init(StrReader *r, const char *s, int len)
{
	r->s = s;
	r->len = len;
	r->cur = 0;
}

int
strreader_getc(StrReader *r)
{
	if (r->cur >= r->len)
		return -1;

	return r->s[r->cur++];
}

int
strreader_getn(StrReader *r, int n, Str *s)
{
	if (r->cur + n > r->len)
		return -1;

	s->s = (char *)r->s + r->cur;
	s->len = n;
	r->cur += n;

	return 0;
}
