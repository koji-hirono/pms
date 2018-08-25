#ifndef BUF_H__
#define BUF_H__

#include <stddef.h>

typedef struct Buf Buf;

struct Buf {
	size_t len;
	size_t cap;
	char *data;
};

extern void buf_init(Buf *, size_t, size_t, void *);
extern int buf_push_u8(Buf *, int);
extern void buf_reverse(Buf *, int, int);

#endif
