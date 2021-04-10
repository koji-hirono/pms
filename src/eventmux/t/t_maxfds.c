#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>

#include "logger.h"
#include "timeex.h"
#include "eventmux.h"
#include "eventselect.h"
#include "eventtimer.h"
#include "eventio.h"


static void
io_handler(EventMux *mux, Event *event)
{
	EventIO *io = event_data(event, EventIO, event);
	//char buf[1];

	(void)mux;

	logger_info("read event fd: %d", io->fd);

	//read(io->fd, buf, sizeof(buf));
}

static void
test1(int n)
{
	EventDriver driver;
	EventMux mux;
	EventIO std;
	EventIO io[n];
	int fds[2];
	int i;

	eventselect_init(&driver);
	eventmux_init(&mux, &driver);

	eventio_init(&std, 0, EVENT_IO_READ, io_handler);
	eventmux_add(&mux, &std.event);

	for (i = 0; i < n; i++) {
		if (socketpair(PF_LOCAL, SOCK_STREAM, 0, fds) != 0) {
			logger_error("socketpair failed. %s", strerror(errno));
			break;
		}
		eventio_init(&io[i], fds[0], EVENT_IO_READ, io_handler);
		eventmux_add(&mux, &io[i].event);
		logger_info("[%d] %d %d created.", i, fds[0], fds[1]);
	}

	if (write(fds[1], "c", 1) < 0) {
		logger_error("write failed. %s", strerror(errno));
		return;
	}

	eventmux_serve(&mux);
}

int
main(int argc, char **argv)
{
	int n = 1024 * 100;

	if (argc >= 2) {
		n = strtoul(argv[1], NULL, 0);
	}

	test1(n);
	return 0;
}
