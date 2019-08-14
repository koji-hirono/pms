#include <stddef.h>

#include "buf.h"
#include "str.h"
#include "inilexer.h"


int
inilexer_open(INILexer *lex, Stream *in)
{
	buf_init(&lex->buf, 64);
	lex->in = in;
	lex->i = 0;

	return 0;
}

void
inilexer_close(INILexer *lex)
{
	buf_free(&lex->buf);
}

int
inilexer_token(INILexer *lex, Str *token)
{
	if (lex->i > 0) {
		if (str_substr(token, &lex->line, lex->i + 1, -1) != 0)
			*token = STR_L("");
		else
			str_trim(token, token, NULL);
		lex->i = 0;
		return INI_VAL;
	}

	while (stream_gets(lex->in, &lex->buf) == 0) {
		str_trim(&lex->line, &STR_S(lex->buf.data), NULL);

		/* blank line */
		if (str_equals(&lex->line, &STR_L("")))
			continue;

		/* comment */
		if (str_startswith(&lex->line, &STR_L(";")) ||
		    str_startswith(&lex->line, &STR_L("#")))
			continue;

		/* section */
		if (str_startswith(&lex->line, &STR_L("["))) {
			str_trim_prefix(token, &lex->line, &STR_L("["));
			str_trim_suffix(token, token, &STR_L("]"));
			str_trim(token, token, NULL);
			return INI_SECT;
		}

		/* key */
		lex->i = str_indexany(&lex->line, &STR_L("=:"));
		if (lex->i == -1)
			continue;
		str_substr(token, &lex->line, 0, lex->i);
		str_trim(token, token, NULL);
		return INI_KEY;
	}

	return -1;
}
