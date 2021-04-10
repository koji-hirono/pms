#include <stdio.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>

#include "timeex.h"
#include "eventmux.h"
#include "eventselect.h"
#include "eventtimer.h"
#include "eventio.h"


static void
print_time(struct timespec *ts)
{
	struct tm tm;
	char buf[80];

	localtime_r(&ts->tv_sec, &tm);
	strftime(buf, sizeof(buf), "%F %T", &tm);
	printf("%s %ld nsec\n", buf, ts->tv_nsec);
}

static void
t1_handler(EventMux *mux, Event *event)
{
	EventTimer *timer = event_data(event, EventTimer, event);
	struct timespec ts;

	(void)mux;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	print_time(&ts);
	printf("timeout\n");

	printf("at: ");
	print_time(&timer->at);

	if (0) {
		event_clear(event);
	} else {
		eventtimer_start(timer);
	}
}

static void
io_handler(EventMux *mux, Event *event)
{
	EventIO *io = event_data(event, EventIO, event);
	char buf[1];

	(void)mux;

	printf("read event\n");

	read(io->fd, buf, sizeof(buf));
}

static void
test1(void)
{
	EventDriver driver;
	EventMux mux;
	EventTimer t1;
	EventIO io;
	struct timespec ts;

	eventselect_init(&driver);
	eventmux_init(&mux, &driver);

	eventtimer_init(&t1, &(struct timespec){2, 0}, t1_handler);
	eventmux_add(&mux, &t1.event);

	eventio_init(&io, 0, EVENT_IO_READ, io_handler);
	eventmux_add(&mux, &io.event);

	clock_gettime(CLOCK_MONOTONIC, &ts);
	print_time(&ts);
	eventtimer_start(&t1);

	eventmux_serve(&mux);
}

int
main(void)
{
	test1();
	return 0;
}
