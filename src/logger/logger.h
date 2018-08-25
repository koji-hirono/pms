#ifndef LOGGER_H__
#define LOGGER_H__

#include <stdarg.h>

typedef struct Loginfo Loginfo;

struct Loginfo {
	const char *level;
	const char *func;
	const char *file;
	int line;
};

#define LOGINFO(level) &(Loginfo){#level, __func__, __FILE__, __LINE__}

#define logger_debug(...) logger_log(LOGINFO(debug), __VA_ARGS__)
#define logger_info(...) logger_log(LOGINFO(info), __VA_ARGS__)
#define logger_warn(...) logger_log(LOGINFO(warn), __VA_ARGS__)
#define logger_error(...) logger_log(LOGINFO(error), __VA_ARGS__)
#define logger_fatal(...) logger_log(LOGINFO(fatal), __VA_ARGS__)

extern void logger_log(const Loginfo *, const char *, ...)
	__attribute__((format(printf, 2, 3)));
extern void logger_vlog(const Loginfo *, const char *, va_list);

#endif
