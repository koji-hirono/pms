#ifndef FDSET_H__
#define FDSET_H__

#include <sys/select.h>

typedef struct FDSet FDSet;

struct FDSet {
	int nfd;
	fd_set *data;
};

extern int fdset_alloc(FDSet *);
extern void fdset_free(FDSet *);
extern int fdset_resize(FDSet *, int);
extern void fdset_clear(FDSet *);

#endif
