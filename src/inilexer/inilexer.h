#ifndef INILEXER_H_
#define INILEXER_H_

#include "buf.h"
#include "str.h"
#include "stream.h"

typedef struct INILexer INILexer;

enum {
	INI_SECT,
	INI_KEY,
	INI_VAL
};

struct INILexer {
	Stream *in;
	Buf buf;
	Str line;
	int i;
};

extern int inilexer_open(INILexer *, Stream *);
extern void inilexer_close(INILexer *);
extern int inilexer_token(INILexer *, Str *);

#endif
