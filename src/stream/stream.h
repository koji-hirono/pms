#ifndef STREAM_H__
#define STREAM_H__

#include <stddef.h>

#include "buf.h"

typedef struct Stream Stream;

struct Stream {
	Buf buf;
	size_t r;
	int fd;
	int eof;
	int error;
};

extern int stream_open(Stream *, int);
extern void stream_close(Stream *);
extern int stream_gets(Stream *, Buf *);
extern int stream_getc(Stream *);
extern int stream_ungetc(Stream *, int);

#endif
