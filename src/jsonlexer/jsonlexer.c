#include <stddef.h>
#include <ctype.h>

#include "buf.h"
#include "stream.h"
#include "jsonlexer.h"


static int parse_escape(JSONLexer *, Buf *);
static int parse_number(JSONLexer *, Buf *);


void
jsonlexer_init(JSONLexer *lexer, Stream *s)
{
	lexer->s = s;
}

int
jsonlexer_token(JSONLexer *lexer, Buf *token)
{
	int c;

	buf_clear(token);

	for (;;) {
		c = stream_getc(lexer->s);
		switch (c) {
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			/* ws */
			continue;
		case ':':
			/* name-separator */
			continue;
		case ',':
			/* value-separator */
			continue;
		case '[':
			/* begin-array */
			return JSON_ARRAY_START;
		case '{':
			/* begin-object */
			return JSON_OBJECT_START;
		case ']':
			/* end-array */
			return JSON_ARRAY_END;
		case '}':
			/* end-object */
			return JSON_OBJECT_END;
		case '"':
			/* string */
			c = stream_getc(lexer->s);
			while (c != -1 && c != '"') {
				buf_pushc(token, c);
				/* escape */
				if (c == '\\') {
					if (parse_escape(lexer, token) != 0)
						return -1;
				}
				c = stream_getc(lexer->s);
			}
			return JSON_STRING;
		case '-':
			/* -number */
			buf_pushc(token, c);
			if (parse_number(lexer, token) != 0)
				return -1;
			return JSON_NUMBER;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			/* number */
			stream_ungetc(lexer->s, c);
			if (parse_number(lexer, token) != 0)
				return -1;
			return JSON_NUMBER;
		case 'n':
			/* null */
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'u')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'l')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'l')
				return -1;
			buf_pushc(token, c);
			return JSON_NULL;
		case 't':
			/* true */
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'r')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'u')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'e')
				return -1;
			buf_pushc(token, c);
			return JSON_BOOLEAN;
		case 'f':
			/* false */
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'a')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'l')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 's')
				return -1;
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
			if (c != 'e')
				return -1;
			buf_pushc(token, c);
			return JSON_BOOLEAN;
		default:
			return -1;
		}
	}
}


static int
parse_escape(JSONLexer *lexer, Buf *token)
{
	int c;
	int i;

	c = stream_getc(lexer->s);

	switch (c) {
	case '"':
	case '\\':
	case '/':
	case 'b':
	case 'f':
	case 'n':
	case 'r':
	case 't':
		buf_pushc(token, c);
		break;
	case 'u':
		buf_pushc(token, c);
		for (i = 0; i < 4; i++) {
			c = stream_getc(lexer->s);
			if (!isxdigit(c))
				return -1;
			buf_pushc(token, c);
		}
		break;
	default:
		return -1;
	}

	return 0;
}

static int
parse_number(JSONLexer *lexer, Buf *token)
{
	int c;

	/* int = 0 | [1-9][0-9]* */
	c = stream_getc(lexer->s);
	if (!isdigit(c))
		return -1;
	do {
		buf_pushc(token, c);
		c = stream_getc(lexer->s);
	} while (isdigit(c));

	/* frac = '.' [0-9]+ */
	if (c == '.') {
		buf_pushc(token, c);
		c = stream_getc(lexer->s);
		if (!isdigit(c))
			return -1;
		do {
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
		} while (isdigit(c));
	}

	/* exp = ('e' | 'E') ['-' | '+'] [0-9]+ */
	if (c == 'e' || c == 'E') {
		buf_pushc(token, c);
		c = stream_getc(lexer->s);
		if (c == '-' || c == '+') {
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
		}
		if (!isdigit(c))
			return -1;
		do {
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
		} while (isdigit(c));
	}

	stream_ungetc(lexer->s, c);

	return 0;
}
