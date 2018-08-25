#include <sys/mman.h>

#include <stddef.h>

#include <unistd.h>

#include "page.h"


void *
page_alloc(size_t npage)
{
	size_t size;
	int prot;
	int flags;
	void *p;

	size = npage * page_size();
	prot = PROT_READ | PROT_WRITE;
	flags = MAP_PRIVATE | MAP_ANONYMOUS;

	p = mmap(NULL, size, prot, flags, -1, 0);
	if (p == MAP_FAILED)
		return NULL;

	return p;
}

void
page_free(void *p, size_t npage)
{
	munmap(p, npage * page_size());
}

size_t
page_size(void)
{
	return sysconf(_SC_PAGESIZE);
}
