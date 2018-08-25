#include <stdio.h>

#include "buf.h"
#include "textfmt.h"


static void
test1(void)
{
	char buf[80];
	Buf b;

	buf_init(&b, 0, sizeof(buf), buf);

	textfmt_uint(&b, 0, 0, 0, 10, -10);
	printf("%s\n", b.data);
}

int
main(void)
{
	test1();
	return 0;
}
