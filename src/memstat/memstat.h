#ifndef DEBUG_MEM_H__
#define DEBUG_MEM_H__

#include <stddef.h>

typedef struct Memstat Memstat;

struct Memstat {
	size_t vs;
	size_t rs;
	size_t text;
	size_t data;
	size_t stack;
};

extern int memstat_get(Memstat *);

#endif
