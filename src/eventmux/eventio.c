#include "eventmux.h"
#include "eventio.h"

static void io_prepare(EventDriver *, Event *);
static int io_check(EventDriver *, Event *);

const EventSpec eventio_spec = {
	.prepare = io_prepare,
	.check = io_check
};

void
eventio_init(EventIO *io, int fd, int events, void (*handler)(EventMux *, Event *))
{
	io->event.spec = &eventio_spec;
	io->event.handler = handler;
	io->events = events;
	io->revents = 0;
	io->fd = fd;
}

static void
io_prepare(EventDriver *driver, Event *event)
{
	EventIO *io = event_data(event, EventIO, event);

	if (io->events & EVENT_IO_READ)
		driver->prepare_io_read(driver, io->fd);
	if (io->events & EVENT_IO_WRITE)
		driver->prepare_io_write(driver, io->fd);
}

static int
io_check(EventDriver *driver, Event *event)
{
	EventIO *io = event_data(event, EventIO, event);

	io->revents = 0;
	if (driver->check_io_read(driver, io->fd))
		io->revents |= EVENT_IO_READ;
	if (driver->check_io_write(driver, io->fd))
		io->revents |= EVENT_IO_WRITE;

	return io->revents != 0;
}
