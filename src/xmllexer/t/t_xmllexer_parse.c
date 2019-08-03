#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buf.h"
#include "stream.h"
#include "xmllexer.h"
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
	XmlLexer lexer;
	Buf token;
	int type;

	xml_lexer_init(&lexer, s);
	buf_init(&token, 64);
	print_memstat();
	while ((type = xml_lexer_token(&lexer, &token)) != -1) {
		print_memstat();
		switch (type) {
		case XML_STAG_NAME:
			/* <name */
			printf("stag name = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_ETAG_NAME:
			/* </name */
			printf("etag name = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_ATTR_NAME:
			/* <stag name="value" */
			printf("attr name = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_ATTR_VALUE:
			/* "value" | 'value' */
			printf("attr value = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_DIRECTIVE_NAME:
			/* <!name */
			printf("directive name = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_PI_NAME:
			/* <?name */
			printf("pi name = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_TAG_END:
			/* [/?!]> */
			printf("tag end = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_COMMENT:
			/* <!-- comment --> */
			printf("comment = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_CDATA:
			/* <![CDATA[ cdata ]]> */
			printf("cdata = '%.*s'\n",
					(int)token.len, token.data);
			break;
		case XML_TEXT:
			printf("text = '%.*s'\n",
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
