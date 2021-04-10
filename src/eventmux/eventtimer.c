#include <time.h>

#include "timeex.h"
#include "eventmux.h"
#include "eventtimer.h"


static void timer_prepare(EventDriver *, Event *);
static int timer_check(EventDriver *, Event *);

const EventSpec eventtimer_spec = {
	.prepare = timer_prepare,
	.check = timer_check
};

void
eventtimer_init(EventTimer *timer, struct timespec *value,
		void (*handler)(EventMux *, Event *))
{
	timer->event.spec = &eventtimer_spec;
	timer->event.handler = handler;

	timer->value = *value;
	timespec_init(&timer->at, 0, 0);
}

void
eventtimer_start(EventTimer *timer)
{
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	timespec_add(&timer->at, &timer->value, &now);
}


static void
timer_prepare(EventDriver *driver, Event *event)
{
	EventTimer *timer = event_data(event, EventTimer, event);

	driver->prepare_timer(driver, &timer->at);
}

static int
timer_check(EventDriver *driver, Event *event)
{
	EventTimer *timer = event_data(event, EventTimer, event);

	return driver->check_timer(driver, &timer->at);
}
