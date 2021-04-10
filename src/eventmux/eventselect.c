#include <sys/select.h>

#include <stdlib.h>
#include <time.h>

#include "fdset.h"
#include "timeex.h"
#include "eventmux.h"
#include "eventselect.h"

typedef struct MuxData MuxData;

struct MuxData {
	FDSet r;
	FDSet w;
	int maxfd;
	struct timespec *tmo;
	struct timespec min;
	int n;
};


static void
setup(EventDriver *driver)
{
	MuxData *a = driver->data;

	fdset_clear(&a->r);
	fdset_clear(&a->w);
	a->maxfd = -1;
	a->tmo = NULL;
	timespec_init(&a->min, -1, -1);
	a->n = 0;
}

static void
cleanup(EventDriver *driver)
{
	(void)driver;
}

static void
await(EventDriver *driver)
{
	MuxData *a = driver->data;
	struct timespec now;

	if (a->tmo && !timespec_iszero(a->tmo)) {
		clock_gettime(CLOCK_MONOTONIC, &now);
		if (timespec_cmp(a->tmo, &now, <)) {
			timespec_init(a->tmo, 0, 0);
		} else {
			timespec_sub(a->tmo, a->tmo, &now);
		}
	}

	a->n = pselect(a->maxfd + 1, a->r.data, a->w.data, NULL, a->tmo, NULL);
}

static void
prepare_io_read(EventDriver *driver, int fd)
{
	MuxData *a = driver->data;

	if (fd > a->maxfd) {
		a->maxfd = fd;
		fdset_resize(&a->r, a->maxfd + 1);
		fdset_resize(&a->w, a->maxfd + 1);
	}

	FD_SET(fd, a->r.data);
}

static void
prepare_io_write(EventDriver *driver, int fd)
{
	MuxData *a = driver->data;

	if (fd > a->maxfd) {
		a->maxfd = fd;
		fdset_resize(&a->r, a->maxfd + 1);
		fdset_resize(&a->w, a->maxfd + 1);
	}

	FD_SET(fd, a->w.data);
}

static int
check_io_read(EventDriver *driver, int fd)
{
	MuxData *a = driver->data;

	return FD_ISSET(fd, a->r.data);
}

static int
check_io_write(EventDriver *driver, int fd)
{
	MuxData *a = driver->data;

	return FD_ISSET(fd, a->w.data);
}

static void
prepare_timer(EventDriver *driver, struct timespec *at)
{
	MuxData *a = driver->data;

	if (timespec_iszero(&a->min))
		return;

	if (a->min.tv_sec == -1 || timespec_cmp(at, &a->min, <)) {
		a->min = *at;
		a->tmo = &a->min;
	}
}

static int
check_timer(EventDriver *driver, struct timespec *at)
{
	struct timespec now;

	(void)driver;

	clock_gettime(CLOCK_MONOTONIC, &now);
	return timespec_cmp(&now, at, >=);
}


int
eventselect_init(EventDriver *driver)
{
	MuxData *a;

	a = malloc(sizeof(MuxData));
	if (a == NULL)
		return -1;
	driver->data = a;

	fdset_alloc(&a->r);
	fdset_alloc(&a->w);

	driver->setup = setup;
	driver->cleanup = cleanup;
	driver->await = await;
	driver->prepare_io_read = prepare_io_read;
	driver->prepare_io_write = prepare_io_write;
	driver->prepare_timer = prepare_timer;
	driver->check_io_read = check_io_read;
	driver->check_io_write = check_io_write;
	driver->check_timer = check_timer;

	return 0;
}
