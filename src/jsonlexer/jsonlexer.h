#ifndef JSONLEXER_H__
#define JSONLEXER_H__

#include "buf.h"
#include "stream.h"

typedef struct JSONLexer JSONLexer;

enum {
	JSON_OBJECT_START,	/* { */
	JSON_OBJECT_END,	/* } */
	JSON_ARRAY_START,	/* [ */
	JSON_ARRAY_END,		/* ] */
	JSON_NULL,		/* null */
	JSON_BOOLEAN,		/* ture | false */
	JSON_STRING,		/* "string" */
	JSON_NUMBER		/* digits | float */
};

struct JSONLexer {
	Stream *s;
};

extern void jsonlexer_init(JSONLexer *, Stream *);
extern int jsonlexer_token(JSONLexer *, Buf *);

#endif
