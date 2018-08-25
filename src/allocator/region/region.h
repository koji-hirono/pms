#ifndef REGION_H__
#define REGION_H__

#include <stddef.h>

typedef struct Region Region;

struct Region {
	void *large;
	void *small;
	size_t used;
};

extern void region_open(Region *);
extern void region_close(Region *);
extern void *region_alloc(Region *, size_t);

#endif
