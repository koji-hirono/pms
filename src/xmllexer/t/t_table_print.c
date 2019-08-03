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

static int
equals(Buf *buf, const char *t)
{
	size_t len;

	len = strlen(t);

	if (len != buf->len)
		return 0;

	if (strncmp(buf->data, t, len) != 0)
		return 0;

	return 1;
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

	printf("Id   Name     Address     Zip\n");
	printf("---- -------- ----------- ----\n");

	while ((type = xml_lexer_token(&lexer, &token)) != -1) {
		if (type != XML_STAG_NAME || !equals(&token, "table")) {
			continue;
		}
		while ((type = xml_lexer_token(&lexer, &token)) != -1) {
			if (type == XML_ETAG_NAME && equals(&token, "table")) {
				break;
			}
			if (type != XML_STAG_NAME || !equals(&token, "row")) {
				continue;
			}
			while ((type = xml_lexer_token(&lexer, &token)) != -1) {
				if (type == XML_ETAG_NAME &&
				    equals(&token, "row")) {
					printf("\n");
					break;
				}
				if (type == XML_STAG_NAME &&
				    equals(&token, "id")) {
					while ((type = xml_lexer_token(&lexer, &token)) != -1) {
						if (type == XML_TEXT)
							break;
					}
					printf("%.*s ",
						(int)token.len, token.data);
					continue;
				}
				if (type == XML_STAG_NAME &&
				    equals(&token, "firstname")) {
					while ((type = xml_lexer_token(&lexer, &token)) != -1) {
						if (type == XML_TEXT)
							break;
					}
					printf("%.*s ",
						(int)token.len, token.data);
					continue;
				}
				if (type == XML_STAG_NAME &&
				    equals(&token, "lastname")) {
					while ((type = xml_lexer_token(&lexer, &token)) != -1) {
						if (type == XML_TEXT)
							break;
					}
					printf("%.*s ",
						(int)token.len, token.data);
					continue;
				}
				if (type == XML_STAG_NAME &&
				    equals(&token, "street")) {
					while ((type = xml_lexer_token(&lexer, &token)) != -1) {
						if (type == XML_TEXT)
							break;
					}
					printf("%.*s ",
						(int)token.len, token.data);
					continue;
				}
			}
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
