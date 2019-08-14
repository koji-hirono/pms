#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "stream.h"
#include "inilexer.h"
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
	INILexer lexer;
	Str token;
	int type;

	inilexer_open(&lexer, s);
	print_memstat();
	while ((type = inilexer_token(&lexer, &token)) != -1) {
		print_memstat();
		switch (type) {
		case INI_SECT:
			printf("section = '%.*s'\n",
				(int)token.len, token.s);
			break;
		case INI_KEY:
			printf("key = '%.*s'\n",
				(int)token.len, token.s);
			break;
		case INI_VAL:
			printf("value = '%.*s'\n",
				(int)token.len, token.s);
			break;
		default:
			break;
		}
	}

	inilexer_close(&lexer);

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
