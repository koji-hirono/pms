#ifndef PARSEC_H__
#define PARSEC_H__

#include <stddef.h>

#include "parsetree.h"
#include "parser.h"

typedef struct Parsec Parsec;

struct Parsec {
	int (*parse)(Parsec *, Parser *, ParseNode **);
	union {
		Parsec **exprs;
		Parsec *expr;
		const char *text;
	};
	int lower;
	int upper;
};


extern int parsec_sequence(Parsec *, Parser *, ParseNode **);
extern int parsec_orderd_choice(Parsec *, Parser *, ParseNode **);
extern int parsec_zero_or_more(Parsec *, Parser *, ParseNode **);
extern int parsec_one_or_more(Parsec *, Parser *, ParseNode **);
extern int parsec_optional(Parsec *, Parser *, ParseNode **);
extern int parsec_and_predicate(Parsec *, Parser *, ParseNode **);
extern int parsec_not_predicate(Parsec *, Parser *, ParseNode **);
extern int parsec_literal(Parsec *, Parser *, ParseNode **);
extern int parsec_char_group(Parsec *, Parser *, ParseNode **);
extern int parsec_eol(Parsec *, Parser *, ParseNode **);
extern int parsec_quant(Parsec *, Parser *, ParseNode **);
extern int parsec_rule(Parsec *, Parser *, ParseNode **);


#define SEQUENCE(...) &(Parsec){ \
	.parse = parsec_sequence, \
	.exprs = (Parsec *[]){__VA_ARGS__, NULL}, \
}

#define ORDERD_CHOICE(...) &(Parsec){ \
	.parse = parsec_orderd_choice, \
	.exprs = (Parsec *[]){__VA_ARGS__, NULL}, \
}

#define ZERO_OR_MORE(e) &(Parsec){ \
	.parse = parsec_zero_or_more, \
	.expr = e, \
}

#define ONE_OR_MORE(e) &(Parsec){ \
	.parse = parsec_one_or_more, \
	.expr = e, \
}

#define OPTIONAL(e) &(Parsec){ \
	.parse = parsec_optional, \
	.expr = e, \
}

#define AND_PREDICATE(e) &(Parsec){ \
	.parse = parsec_and_predicate, \
	.expr = e, \
}

#define NOT_PREDICATE(e) &(Parsec){ \
	.parse = parsec_not_predicate, \
	.expr = e, \
}

#define LITERAL(s) &(Parsec){ \
	.parse = parsec_literal, \
	.text = s, \
}

#define CHAR_GROUP(s) &(Parsec){ \
	.parse = parsec_char_group, \
	.text = s, \
}

#define EOL() &(Parsec){ \
	.parse = parsec_eol, \
}

#define QUANT(e,n,m) &(Parsec){ \
	.parse = parsec_quant, \
	.expr = e, \
	.lower = n, \
	.upper = m, \
}

#define QUANT_MIN(e,n) QUANT(e,n,-1)
#define QUANT_MAX(e,m) QUANT(e,-1,m)

#define RULE() &(Parsec){ \
	.parse = parsec_rule, \
}

#define RULE_DEF(c,e) (c)->expr = e

#endif
