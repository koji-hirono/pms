#include <stddef.h>
#include <string.h>

#include "parser.h"
#include "parsetree.h"


void
parser_init(Parser *parser, const char *text)
{
	parser->text = text;
	parser->pos = 0;
	parser->len = strlen(text);
}
