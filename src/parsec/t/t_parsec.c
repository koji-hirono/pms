#include <stdio.h>
#include <string.h>

#include "parsec.h"

static void
parsetree_print(ParseNode *node, int depth)
{
	int i;

	if (node == NULL)
		return;

	switch (node->type) {
	case 'L':
		printf("%*s", depth * 2, "");
		printf("[\n");
		for (i = 0; i < node->list.n; i++)
			parsetree_print(node->list.data[i], depth + 1);
		printf("%*s", depth * 2, "");
		printf("]\n");
		break;
	case 'S':
		printf("%*s", depth * 2, "");
		printf("%.*s\n", (int)node->str.len, node->str.s);
		break;
	default:
		break;
	}
}

int
main(void)
{
	/*
	 * Expr    = Sum
	 * Sum     = Product (('+' / '-') Product)*
	 * Product = Value (('*' / '/') Value)*
	 * Value   = [0-9]+ / '(' Expr ')'
	 */
	Parsec *g = RULE();
	Parsec *expr = RULE();
	Parsec *sum = RULE();
	Parsec *product = RULE();
	Parsec *value = RULE();

	RULE_DEF(g, SEQUENCE(expr, EOL()));
	RULE_DEF(expr, sum);
	RULE_DEF(sum,
		SEQUENCE(
			product,
			ZERO_OR_MORE(
				SEQUENCE(
					ORDERD_CHOICE(LITERAL("+"), LITERAL("-")),
				       	product
				)
			)
		)
	);
	RULE_DEF(product,
		SEQUENCE(
			value,
			ZERO_OR_MORE(
				SEQUENCE(
					ORDERD_CHOICE(LITERAL("*"), LITERAL("/")),
				       	value
				)
			)
		)
	);
	RULE_DEF(value,
		ORDERD_CHOICE(
			ONE_OR_MORE(CHAR_GROUP("0123456789")),
			SEQUENCE(LITERAL("("), expr, LITERAL(")"))
		)
	);
	Parser parser;
	ParseNode *t;
	int accepted;

	parser_init(&parser, "9-4-3");

	accepted = g->parse(g, &parser, &t);

	printf("rule G(%p)\n", g);
	printf("rule Expr(%p)\n", expr);
	printf("rule Sum(%p)\n", sum);
	printf("rule Product(%p)\n", product);
	printf("rule Value(%p)\n", value);

	printf("accepted: %d\n", accepted);
	printf("node    : %p\n", t);

	parsetree_print(t, 0);

	parsetree_free(t);

	return 0;
}
