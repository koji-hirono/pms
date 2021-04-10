#include <sys/select.h>

#include <stdlib.h>

#include "fdset.h"


int
fdset_alloc(FDSet *set)
{
	set->data = malloc(sizeof(fd_set));
	if (set->data == NULL)
		return -1;

	FD_ZERO(set->data);

	set->nfd = FD_SETSIZE;

	return 0;
}

void
fdset_free(FDSet *set)
{
	free(set->data);
}

int
fdset_resize(FDSet *set, int nfd)
{
	void *data;
	int oldn;
	int n;
	int i;

	oldn = (set->nfd + (FD_SETSIZE - 1)) / FD_SETSIZE;
	n = (nfd + (FD_SETSIZE - 1)) / FD_SETSIZE;

	if (n <= oldn)
		return 0;

	data = realloc(set->data, n * sizeof(fd_set));
	if (data == NULL)
		return -1;

	set->data = data;
	set->nfd = n * FD_SETSIZE;

	for (i = oldn; i < n; i++)
		FD_ZERO(set->data + i);

	return 0;
}

void
fdset_clear(FDSet *set)
{
	int n = (set->nfd + (FD_SETSIZE - 1)) / FD_SETSIZE;
	int i;

	for (i = 0; i < n; i++)
		FD_ZERO(set->data + i);
}
