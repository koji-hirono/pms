#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "logger.h"


static void
func1(void)
{
	int p;

	logger_info("start");

	logger_debug("%p", &p);

	logger_info("end");
	
}

static void
func2(void)
{
	logger_info("start");

	func1();

	logger_error("failed. %s", strerror(EINVAL));

	logger_info("end");
}

int
main(void)
{
	logger_info("start");

	func1();
	func2();

	logger_info("end");

	return 0;
}
