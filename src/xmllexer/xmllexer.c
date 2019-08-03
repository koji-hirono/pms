#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "buf.h"
#include "stream.h"
#include "xmllexer.h"


typedef struct EntityChar EntityChar;

struct EntityChar {
	const char *name;
	int code;
};

const EntityChar entity_char_table[] = {
	{"amp", '&'},
	{"lt", '<'},
	{"gt", '>'},
	{"apos", '\''},
	{"quot", '"'},
	{NULL, -1}
};

static const EntityChar *
entity_char_find(const char *s, int len)
{
	const EntityChar *e;

	for (e = entity_char_table; e->name != NULL; e++)
		if (strncmp(s, e->name, len) == 0)
			return e;

	return NULL;
}

static int
xml_is_space(int c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static int
xml_is_name_start(int c)
{
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c == '_')
		return 1;
	if (c == ':')
		return 1;
	return 0;
}

static int
xml_is_name(int c)
{
	if (xml_is_name_start(c))
		return 1;
 	if (c == '-')
		return 1;
	if (c == '.')
		return 1;
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
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

static int
endswith(Buf *buf, const char *t)
{
	size_t len;

	len = strlen(t);

	if (len > buf->len)
		return 0;

	if (strncmp(buf->data + buf->len - len, t, len) != 0)
		return 0;

	return 1;
}

static int
parse_reference(XmlLexer *lexer)
{
	const EntityChar *e;
	Buf name;
	int c;

	buf_init(&name, 8);
	c = stream_getc(lexer->s);
	while (c != -1 && c != ';') {
		buf_pushc(&name, c);
		c = stream_getc(lexer->s);
	}

	e = entity_char_find(name.data, name.len);
	if (e) {
		buf_free(&name);
		return e->code;
	}

	buf_free(&name);
	return -1;
}

static int
xml_state_out(XmlLexer *lexer, Buf *token)
{
	int c;

	c = stream_getc(lexer->s);
	if (c == -1)
		return -1;
	if (c == '<') {
		c = stream_getc(lexer->s);
		if (c == -1)
			return -1;
		if (c == '?') {
			/* XMLDecl | PI */
			c = stream_getc(lexer->s);
			while (xml_is_name(c)) {
				buf_pushc(token, c);
				c = stream_getc(lexer->s);
			}
			stream_ungetc(lexer->s, c);
			lexer->state = XML_STATE_IN_TAG;
			return XML_PI_NAME;
		}
		if (c == '!') {
			c = stream_getc(lexer->s);
			if (c == -1)
				return -1;
			if (c == '-') {
				/* Comment */
				c = stream_getc(lexer->s);
				if (c == -1)
					return -1;
				if (c != '-')
					return -1;
				/* comment --> */
				c = stream_getc(lexer->s);
				while (c != -1) {
					buf_pushc(token, c);
					if (endswith(token, "-->")) {
						token->len -= 3;
						break;
					}
					c = stream_getc(lexer->s);
				}
				return XML_COMMENT;
			}
			if (c == '[') {
				Buf name;
				buf_init(&name, 8);
				/* CDSect */
				/* CDATA[ cdata ]]> */
				c = stream_getc(lexer->s);
				while (xml_is_name(c)) {
					buf_pushc(&name, c);
					c = stream_getc(lexer->s);
				}
				if (!equals(&name, "CDATA")) {
					buf_free(&name);
					return -1;
				}
				buf_free(&name);
				if (c != '[')
					return -1;
				c = stream_getc(lexer->s);
				while (c != -1) {
					buf_pushc(token, c);
					if (endswith(token, "]]>")) {
						token->len -= 3;
						break;
					}
					c = stream_getc(lexer->s);
				}
				return XML_CDATA;
			}
			/* doctypedecl */
			while (xml_is_name(c)) {
				buf_pushc(token, c);
				c = stream_getc(lexer->s);
			}
			stream_ungetc(lexer->s, c);
			lexer->state = XML_STATE_IN_TAG;
			return XML_DIRECTIVE_NAME;
		}
		if (c == '/') {
			/* ETag */
			c = stream_getc(lexer->s);
			while (xml_is_name(c)) {
				buf_pushc(token, c);
				c = stream_getc(lexer->s);
			}
			stream_ungetc(lexer->s, c);
			lexer->state = XML_STATE_IN_TAG;
			return XML_ETAG_NAME;
		}
		/* EmptyElmTag */
		/* STag */
		while (xml_is_name(c)) {
			buf_pushc(token, c);
			c = stream_getc(lexer->s);
		}
		stream_ungetc(lexer->s, c);
		lexer->state = XML_STATE_IN_TAG;
		return XML_STAG_NAME;
	}
	/* text */
	do {
		if (c == '&') {
			c = parse_reference(lexer);
			if (c == -1)
				break;
		}
		buf_pushc(token, c);
		c = stream_getc(lexer->s);
	} while (c != '<' && c != -1);
	stream_ungetc(lexer->s, c);
	return XML_TEXT;
}

static int
xml_state_in_tag(XmlLexer *lexer, Buf *token)
{
	int c;

	c = stream_getc(lexer->s);
	/* skip space */
	while (xml_is_space(c))
		c = stream_getc(lexer->s);
	if (c == -1)
		return -1;
	if (c == '>') {
		lexer->state = XML_STATE_OUT;
		return XML_TAG_END;
	}
	if (c == '?' || c == '/') {
		buf_pushc(token, c);
		c = stream_getc(lexer->s);
		if (c == -1)
			return -1;
		if (c == '>') {
			lexer->state = XML_STATE_OUT;
			return XML_TAG_END;
		}
		/* error */
		return -1;
	}
	if (c == '=') {
		/* attribute value */
		c = stream_getc(lexer->s);
		/* skip space */
		while (xml_is_space(c))
			c = stream_getc(lexer->s);
		if (c == -1)
			return -1;
		if (c == '"') {
			c = stream_getc(lexer->s);
			while (c != '"' && c != -1) {
				if (c == '&') {
					c = parse_reference(lexer);
					if (c == -1)
						break;
				}
				buf_pushc(token, c);
				c = stream_getc(lexer->s);
			}
			return XML_ATTR_VALUE;
		}
		if (c == '\'') {
			c = stream_getc(lexer->s);
			while (c != '\'' && c != -1) {
				if (c == '&') {
					c = parse_reference(lexer);
					if (c == -1)
						break;
				}
				buf_pushc(token, c);
				c = stream_getc(lexer->s);
			}
			return XML_ATTR_VALUE;
		}
		/* error */
		return -1;
	}
	/* attribute name */
	while (xml_is_name(c)) {
		buf_pushc(token, c);
		c = stream_getc(lexer->s);
	}
	stream_ungetc(lexer->s, c);
	return XML_ATTR_NAME;
}

void
xml_lexer_init(XmlLexer *lexer, Stream *s)
{
	lexer->s = s;
	lexer->state = XML_STATE_OUT;
}

int
xml_lexer_token(XmlLexer *lexer, Buf *token)
{
	buf_clear(token);

	switch (lexer->state) {
	case XML_STATE_OUT:
		return xml_state_out(lexer, token);
	case XML_STATE_IN_TAG:
		return xml_state_in_tag(lexer, token);
	default:
		return -1;
	}
}
