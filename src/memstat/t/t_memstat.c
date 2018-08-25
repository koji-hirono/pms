#include <stdio.h>

#include <unistd.h>

#include "memstat.h"

static void
print_memstat(Memstat *m)
{
#if 0
	if (write(1, m, sizeof(*m)) <= 0)
		return;
#else
	printf("vmsize   : %zu\n", m->vs);
	printf("rssize   : %zu\n", m->rs);
	printf("textsize : %zu\n", m->text);
	printf("datasize : %zu\n", m->data);
	printf("stacksize: %zu\n", m->stack);
#endif
}

int
main(void)
{
	Memstat m;

	memstat_get(&m);
	print_memstat(&m);

	memstat_get(&m);
	print_memstat(&m);

	return 0;
}
