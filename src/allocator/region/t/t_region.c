#include <stdio.h>
#include <stdlib.h>

#include "memstat.h"
#include "region.h"

typedef struct Block Block;

struct Block {
	Block *next;
	size_t npage;
};

static void
print_block(Block *b)
{
	printf("block [%p] = {", b);
	printf("next = %p, npage = %zu", b->next, b->npage);
	printf("}\n");
}

static void
print_region(Region *r)
{
	Block *b;

	printf("large\n");
	for (b = r->large; b != NULL; b = b->next) {
		print_block(b);
	}

	printf("small\n");
	for (b = r->small; b != NULL; b = b->next) {
		print_block(b);
	}

	printf("used = %zu\n", r->used);
}

int
main(void)
{
	Memstat m;
	Region r;
	size_t size;
	void *p;
	int i;
	int n;

	memstat_get(&m);
	printf("vsize: %zu, rssize: %zu\n", m.vs, m.rs);

	memstat_get(&m);
	printf("vsize: %zu, rssize: %zu\n", m.vs, m.rs);

	n = 100;

	region_open(&r);

	for (i = 1; i <= n; i++) {
		size = i * i;
		p = region_alloc(&r, size);
		printf("p = %p, size = %zu\n", p, size);
		memstat_get(&m);
		printf("vsize: %zu, rssize: %zu\n", m.vs, m.rs);
	}

	print_region(&r);

	region_close(&r);

	memstat_get(&m);
	printf("vsize: %zu, rssize: %zu\n", m.vs, m.rs);

	return 0;
}
