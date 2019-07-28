#include <stddef.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "buf.h"
#include "stream.h"


int
stream_open(Stream *s, int fd)
{
	if (buf_init(&s->buf, 4096) != 0)
		return -1;

	s->fd = fd;
	s->r = 0;
	s->eof = 0;
	s->error = 0;

	return 0;
}

void
stream_close(Stream *s)
{
	buf_free(&s->buf);
}

int
stream_getc(Stream *s)
{
	int c;
	int r;

	if (s->eof || s->error)
		return -1;

	if (s->r >= s->buf.len) {
		r = read(s->fd, s->buf.data, s->buf.cap);
		if (r == -1) {
			s->error = errno;
			return -1;
		}
		if (r == 0) {
			s->eof = 1;
			return -1;
		}
		s->buf.len = r;
		s->r = 0;
	}

	c = s->buf.data[s->r];
	s->r++;
	return c;
}

int
stream_ungetc(Stream *s, int c)
{
	if (s->r == 0)
		return -1;
	if (c == -1)
		return -1;

	s->r--;
	return 0;
}
