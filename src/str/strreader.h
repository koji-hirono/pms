#ifndef STRREADER_H__
#define STRREADER_H__

#include "str.h"

typedef struct StrReader StrReader;

struct StrReader {
	const char *s;
	int len;
	int cur;
};

extern void strreader_init(StrReader *, const char *, int);
extern int strreader_getc(StrReader *);
extern int strreader_getn(StrReader *, int, Str *);

#endif
