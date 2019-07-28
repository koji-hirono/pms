#include <stdio.h>

#include "buf.h"
#include "textfmt.h"


static void
test1(void)
{
	Buf b;

	buf_init(&b, 80);

	textfmt_uint(&b, 0, 0, 0, 10, -10);
	printf("%s\n", b.data);

	buf_free(&b);
}

int
main(void)
{
	test1();
	return 0;
}
