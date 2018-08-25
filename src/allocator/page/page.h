#ifndef PAGE_H__
#define PAGE_H__

#include <stddef.h>

extern void *page_alloc(size_t);
extern void page_free(void *, size_t);
extern size_t page_size(void);

#endif
