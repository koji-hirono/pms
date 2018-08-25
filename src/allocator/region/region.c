#include <inttypes.h>
#include <stddef.h>

#include "page.h"
#include "region.h"

typedef struct Block Block;

struct Block {
	Block *next;
	size_t npage;
};


void
region_open(Region *r)
{
	r->large = NULL;
	r->small = NULL;
	r->used = 0;
}

void
region_close(Region *r)
{
	Block *next;
	Block *b;

	for (b = r->large; b != NULL; b = next) {
		next = b->next;
		page_free(b, b->npage);
	}

	for (b = r->small; b != NULL; b = next) {
		next = b->next;
		page_free(b, b->npage);
	}
}

void *
region_alloc(Region *r, size_t size)
{
	size_t npage;
	Block *b;
	void *p;

	/* roundup 8 byte */
	size = (size + 0x7) & ~0x7;

	npage = (size + sizeof(Block)) / page_size();

	if (npage != 0) {
		b = page_alloc(npage);
		if (b == NULL)
			return NULL;
		b->npage = npage;
		b->next = r->large;
		r->large = b;
		return b + 1;
	}

	npage = 1;

	if (r->small == NULL ||
	    size >= page_size() - sizeof(Block) - r->used) {
		b = page_alloc(npage);
		if (b == NULL)
			return NULL;
		b->npage = npage;
		b->next = r->small;
		r->small = b;
		r->used = 0;
	} else {
		b = r->small;
	}

	p = (char *)(b + 1) + r->used;
	r->used += size;

	return p;
}
