#ifndef PARSER_H__
#define PARSER_H__

typedef struct Parser Parser;

struct Parser {
	int len;
	int pos;
	const char *text;
};

extern void parser_init(Parser *, const char *);

#endif
