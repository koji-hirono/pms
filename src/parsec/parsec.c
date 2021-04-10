#include <stdio.h>
#include <stddef.h>

#include "parsetree.h"
#include "parsec.h"


int
parsec_sequence(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec **expr;
	ParseNode *ts;
	ParseNode *tc;
	int accepted;
	int pos;

	expr = c->exprs;
	if (!expr)
		return 0;

	if (t)
		ts = parsetree_list_create();

	pos = parser->pos;
	for (; *expr; expr++) {
		accepted = (*expr)->parse(*expr, parser, &tc);
		if (!accepted) {
			parser->pos = pos;
			if (t) {
				parsetree_free(ts);
				*t = NULL;
			}
			return 0;
		}
		if (t)
			parsetree_list_append(ts, tc);
	}

	if (t)
		*t = ts;

	return 1;
}

int
parsec_orderd_choice(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec **expr;
	int accepted;
	int pos;

	expr = c->exprs;
	if (!expr)
		return 0;

	pos = parser->pos;
	for (; *expr; expr++) {
		accepted = (*expr)->parse(*expr, parser, t);
		if (accepted) {
			return 1;
		}
		parser->pos = pos;
	}

	return 0;
}

int
parsec_zero_or_more(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	ParseNode *ts;
	ParseNode *tc;
	int accepted;
	int pos;

	expr = c->expr;
	if (!expr)
		return 0;

	if (t)
		ts = NULL;

	for (;;) {
		pos = parser->pos;
		accepted = expr->parse(expr, parser, &tc);
		if (!accepted) {
			parser->pos = pos;
			break;
		}
		if (t) {
			if (!ts)
				ts = parsetree_list_create();
			parsetree_list_append(ts, tc);
		}
	}

	if (t)
		*t = ts;

	return 1;
}

int
parsec_one_or_more(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	ParseNode *ts;
	ParseNode *tc;
	int accepted;
	int pos;
	int n;

	expr = c->expr;
	if (!expr)
		return 0;

	if (t)
		ts = parsetree_list_create();

	n = 0;
	for (;;) {
		pos = parser->pos;
		accepted = expr->parse(expr, parser, &tc);
		if (!accepted) {
			parser->pos = pos;
			break;
		}
		n++;
		if (t)
			parsetree_list_append(ts, tc);
	}

	if (n < 1) {
		if (t)
			parsetree_free(ts);
		return 0;
	}

	if (t)
		*t = ts;

	return 1;
}

int
parsec_optional(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	int accepted;
	int pos;

	expr = c->expr;
	if (!expr)
		return 0;

	pos = parser->pos;
	accepted = expr->parse(expr, parser, t);
	if (!accepted) {
		parser->pos = pos;
	}

	return 1;
}

int
parsec_and_predicate(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	int accepted;
	int pos;

	if (t)
		*t = NULL;

	expr = c->expr;
	if (!expr)
		return 0;

	pos = parser->pos;
	accepted = expr->parse(expr, parser, NULL);
	parser->pos = pos;

	return accepted;
}

int
parsec_not_predicate(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	int accepted;
	int pos;

	if (t)
		*t = NULL;

	expr = c->expr;
	if (!expr)
		return 0;

	pos = parser->pos;
	accepted = expr->parse(expr, parser, NULL);
	parser->pos = pos;

	return !accepted;
}

int
parsec_literal(Parsec *c, Parser *parser, ParseNode **t)
{
	const char *p;

	if (t)
		*t = NULL;

	printf("parse literal: %.*s = %s\n", (int)strlen(c->text),
		       	parser->text + parser->pos, c->text);

	for (p = c->text; *p != '\0'; p++, parser->pos++) {
		if (parser->pos >= parser->len)
			return 0;
		if (parser->text[parser->pos] != *p)
			return 0;
	}

	printf("literal: %s\n", c->text);

	if (t)
		*t = parsetree_str_create(c->text, p - c->text);

	return 1;
}

int
parsec_char_group(Parsec *c, Parser *parser, ParseNode **t)
{
	const char *p;
	int ch;

	if (t)
		*t = NULL;

	if (parser->pos >= parser->len)
		return 0;

	ch = parser->text[parser->pos];

	printf("char_group: %c\n", ch);

	for (p = c->text; *p != '\0'; p++) {
		if (ch == *p) {
			printf("char_group: %c accepted.\n", *p);
			if (t)
				*t = parsetree_str_create(
						parser->text + parser->pos, 1);
			parser->pos++;
			return 1;
		}
	}

	return 0;
}

int
parsec_eol(Parsec *c, Parser *parser, ParseNode **t)
{
	(void)c;

	if (t)
		*t = NULL;

	return parser->pos == parser->len;
}

int
parsec_quant(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	ParseNode *ts;
	ParseNode *tc;
	int accepted;
	int pos;
	int n;

	expr = c->expr;
	if (!expr)
		return 0;

	if (t)
		ts = parsetree_list_create();

	n = 0;
	for (;;) {
		pos = parser->pos;
		accepted = expr->parse(expr, parser, &tc);
		if (!accepted) {
			parser->pos = pos;
			break;
		}
		n++;
		if (t)
			parsetree_list_append(ts, tc);
		if (c->upper != -1 && n > c->upper) {
			parser->pos = pos;
			if (t)
				parsetree_free(ts);
			return 0;
		}
	}

	if (c->lower != -1 && n < c->lower) {
		if (t)
			parsetree_free(ts);
		return 0;
	}

	if (t && n == 0) {
		parsetree_free(ts);
		*t = NULL;
	}

	return 1;
}

int
parsec_rule(Parsec *c, Parser *parser, ParseNode **t)
{
	Parsec *expr;
	int accepted;
	int pos;

	expr = c->expr;
	if (!expr)
		return 0;

	printf("rule %p\n", c);

	pos = parser->pos;
	accepted = expr->parse(expr, parser, t);
	if (!accepted) {
		printf("rule %p not accepted\n", c);
		parser->pos = pos;
		return 0;
	}
	printf("rule %p accepted\n", c);

	return 1;
}
