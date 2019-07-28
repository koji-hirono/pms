#ifndef BUF_H__
#define BUF_H__

#include <stddef.h>

typedef struct Buf Buf;

struct Buf {
	size_t len;
	size_t cap;
	char *data;
};

extern int buf_init(Buf *, size_t);
extern void buf_free(Buf *);
extern int buf_expand(Buf *, size_t);
extern void buf_clear(Buf *);
extern void buf_reverse(Buf *, int, int);
extern int buf_pushc(Buf *, int);
extern int buf_pushstr(Buf *, const char *);
extern int buf_pushstrz(Buf *, const char *);
extern int buf_pushoct(Buf *, const void *, size_t);

#endif
