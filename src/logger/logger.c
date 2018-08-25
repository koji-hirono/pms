#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"


void
logger_log(const Loginfo *info, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	logger_vlog(info, fmt, ap);
	va_end(ap);
}

void
logger_vlog(const Loginfo *info, const char *fmt, va_list ap)
{
	const char *file;
	char buf[80];
	struct timespec t;

	file = strrchr(info->file, '/');
	if (file != NULL)
		file++;
	else
		file = info->file;

	clock_gettime(CLOCK_REALTIME, &t);
	strftime(buf, sizeof(buf), "%F %T", localtime(&t.tv_sec));
	printf("%s%c%06ld ", buf, '.', t.tv_nsec / 1000L);
	printf("%s ", info->level);
	printf("<%s:%d:%s> ", file, info->line, info->func);
	vprintf(fmt, ap);
	printf("\n");
}
