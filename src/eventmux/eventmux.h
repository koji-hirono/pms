#ifndef EVENTMUX_H__
#define EVENTMUX_H__

#include <time.h>

#include "list.h"

typedef struct EventDriver EventDriver;
typedef struct EventSpec EventSpec;
typedef struct Event Event;
typedef struct EventMux EventMux;

struct EventDriver {
	void (*setup)(EventDriver *);
	void (*cleanup)(EventDriver *);
	void (*await)(EventDriver *);
	void (*prepare_io_read)(EventDriver *, int);
	void (*prepare_io_write)(EventDriver *, int);
	void (*prepare_timer)(EventDriver *, struct timespec *);
	int (*check_io_read)(EventDriver *, int);
	int (*check_io_write)(EventDriver *, int);
	int (*check_timer)(EventDriver *, struct timespec *);
	void *data;
};

struct EventSpec {
	void (*prepare)(EventDriver *, Event *);
	int (*check)(EventDriver *, Event *);
};

struct Event {
	Link source;
	Link target;
	Link ditect;
	const EventSpec *spec;
	void *data;
	void (*handler)(EventMux *, Event *);
};

struct EventMux {
	List source;
	List target;
	List ditect;
	EventDriver *driver;
};

#define event_data(p, type, member) \
	(type *)((char *)p - offsetof(type, member))

extern void eventmux_init(EventMux *, EventDriver *);
extern void eventmux_serve(EventMux *);
extern void eventmux_add(EventMux *, Event *);

extern void event_clear(Event *);

#endif
