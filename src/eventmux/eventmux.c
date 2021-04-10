#include <time.h>

#include "list.h"
#include "eventmux.h"


static void eventmux_prepare(EventMux *);
static void eventmux_check(EventMux *);
static void eventmux_dispatch(EventMux *);


void
eventmux_init(EventMux *mux, EventDriver *driver)
{
	list_init(&mux->source);
	list_init(&mux->target);
	list_init(&mux->ditect);
	mux->driver = driver;
}

void
eventmux_serve(EventMux *mux)
{
	EventDriver *driver = mux->driver;

	for (;;) {
		driver->setup(driver);
		eventmux_prepare(mux);
		driver->await(driver);
		eventmux_check(mux);
		eventmux_dispatch(mux);
		driver->cleanup(driver);
	}
}

void
eventmux_add(EventMux *mux, Event *event)
{
	list_push_back(&mux->source, &event->source);
	list_push_back(&mux->target, &event->target);
}

void
event_clear(Event *event)
{
	list_remove(&event->target);
}

static void
eventmux_prepare(EventMux *mux)
{
	EventDriver *driver = mux->driver;
	Link *target;
	Event *event;

	for (target = list_front(&mux->target);
		target != &mux->target.root;
		target = target->next) {
		event = list_data(target, Event, target);
		event->spec->prepare(driver, event);
	}
}

static void
eventmux_check(EventMux *mux)
{
	EventDriver *driver = mux->driver;
	Link *target;
	Event *event;

	for (target = list_front(&mux->target);
	     target != &mux->target.root;
	     target = target->next) {
		event = list_data(target, Event, target);
		if (event->spec->check(driver, event))
			list_push_back(&mux->ditect, &event->ditect);
	}
}

static void
eventmux_dispatch(EventMux *mux)
{
	Link *ditect;
	Event *event;

	while ((ditect = list_pop_front(&mux->ditect)) != NULL) {
		event = list_data(ditect, Event, ditect);
		event->handler(mux, event);
	}
}
