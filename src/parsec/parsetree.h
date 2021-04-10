#ifndef PARSETREE_H__
#define PARSETREE_H__

#include <stddef.h>

#include "str.h"

typedef struct ParseNode ParseNode;
typedef struct ParseList ParseList;

struct ParseList {
	int n;
	struct ParseNode **data;
};

struct ParseNode {
	int type;
	union {
		ParseList list;
		Str str;
	};
};

extern ParseNode *parsetree_list_create(void);
extern ParseNode *parsetree_str_create(const char *, size_t);
extern void parsetree_free(ParseNode *);
extern int parsetree_list_append(ParseNode *, ParseNode *);

#endif
