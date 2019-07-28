#ifndef XMLLEXER_H__
#define XMLLEXER_H__

#include <stddef.h>

#include "buf.h"
#include "stream.h"

typedef struct XmlLexer XmlLexer;

enum {
	XML_STAG_NAME,		/* <name */
	XML_ETAG_NAME,		/* </name */
	XML_ATTR_NAME,		/* <stag name="value" */
	XML_ATTR_VALUE, 	/* "value" | 'value' */
	XML_DIRECTIVE_NAME,	/* <!name */
	XML_PI_NAME,		/* <?name */
	XML_TAG_END,		/* [/?]> */
	XML_COMMENT,		/* <!-- comment --> */
	XML_CDATA,		/* <![CDATA[ cdata ]]> */
	XML_TEXT
};

enum {
	XML_STATE_OUT,
	XML_STATE_IN_TAG
};

struct XmlLexer {
	Stream *s;
	int state;
};

extern void xml_lexer_init(XmlLexer *, Stream *);
extern int xml_lexer_token(XmlLexer *, Buf *);

#endif
