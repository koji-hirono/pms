#include <stddef.h>

#include "list.h"


void
list_init(List *list)
{
	list->root.next = &list->root;
	list->root.prev = &list->root;
}

Link *
list_front(List *list)
{
	return list->root.next;
}

Link *
list_back(List *list)
{
	return list->root.prev;
}

void
list_push_front(List *list, Link *link)
{
	link->prev = &list->root;
	link->next = list->root.next;
	list->root.next->prev = link;
	list->root.next = link;
}

void
list_push_back(List *list, Link *link)
{
	link->next = &list->root;
	link->prev = list->root.prev;
	list->root.prev->next = link;
	list->root.prev = link;
}

Link *
list_pop_front(List *list)
{
	Link *link;

	link = list->root.next;
	if (link == &list->root)
		return NULL;

	link->next->prev = &list->root;
	list->root.next = link->next;

	return link;
}

Link *
list_pop_back(List *list)
{
	Link *link;

	link = list->root.prev;
	if (link == &list->root)
		return NULL;

	link->prev->next = &list->root;
	list->root.prev = link->prev;

	return link;
}

void
list_remove(Link *link)
{
	link->prev->next = link->next;
	link->next->prev = link->prev;
}
