#ifndef LIST_H__
#define LIST_H__

#include <stddef.h>

typedef struct Link Link;
typedef struct List List;

struct Link {
	Link *next;
	Link *prev;
};

struct List {
	Link root;
};

#define list_data(link, type, member) \
	(type *)((char *)(link) - offsetof(type, member))

extern void list_init(List *);
extern Link *list_front(List *);
extern Link *list_back(List *);
extern void list_push_front(List *, Link *);
extern void list_push_back(List *, Link *);
extern Link *list_pop_front(List *);
extern Link *list_pop_back(List *);
extern void list_remove(Link *);

#endif
