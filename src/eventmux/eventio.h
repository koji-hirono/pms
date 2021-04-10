#ifndef EVENTIO_H__
#define EVENTIO_H__

#include "eventmux.h"

typedef struct EventIO EventIO;

struct EventIO {
	Event event;
	short events;
	short revents;
	int fd;
};

#define EVENT_IO_READ	(1 << 0)
#define EVENT_IO_WRITE	(1 << 1)

extern void eventio_init(EventIO *, int, int, void (*)(EventMux *, Event *));

#endif
