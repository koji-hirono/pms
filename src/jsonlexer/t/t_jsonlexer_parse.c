#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buf.h"
#include "stream.h"
#include "jsonlexer.h"
#include "memstat.h"


static void
print_memstat(void)
{
	Memstat m;

	memstat_get(&m);
	printf("vsize: %zu, rssize: %zu\n", m.vs, m.rs);
}

int
parse(Stream *s)
{
	JSONLexer lexer;
	Buf token;
	int type;

	jsonlexer_init(&lexer, s);
	buf_init(&token, 64);
	print_memstat();
	while ((type = jsonlexer_token(&lexer, &token)) != -1) {
		print_memstat();
		switch (type) {
		case JSON_OBJECT_START:
			/* { */
			printf("object start = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_OBJECT_END:
			/* } */
			printf("object end = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_ARRAY_START:
			/* [ */
			printf("array start = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_ARRAY_END:
			/* ] */
			printf("array end = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_NULL:
			/* null */
			printf("null = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_BOOLEAN:
			/* true | false */
			printf("boolean = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_STRING:
			/* "string" */
			printf("string = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case JSON_NUMBER:
			/* number */
			printf("number = '%.*s'\n",
					(int)token.len, token.data);
			break;
		default:
			break;
		}
	}
	buf_free(&token);

	return 0;
}

static void
test1(void)
{
	Stream s;

	print_memstat();

	stream_open(&s, 0);

	print_memstat();

	parse(&s);

	stream_close(&s);

	print_memstat();
}

int
main(void)
{
	test1();

	return 0;
}
