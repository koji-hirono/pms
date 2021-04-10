#ifndef EVENTTIMER_H__
#define EVENTTIMER_H__

#include <time.h>

#include "eventmux.h"

typedef struct EventTimer EventTimer;

struct EventTimer {
	Event event;
	struct timespec value;
	struct timespec at;
};

extern void eventtimer_init(EventTimer *, struct timespec *,
		void (*)(EventMux *, Event *));
extern void eventtimer_start(EventTimer *);

#endif
