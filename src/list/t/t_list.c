#include <stdio.h>

#include "list.h"

static void
test1(void)
{
	List list;
	Link elm[10];
	Link *e;
	int i;

	for (i = 0; i < 10; i++) {
		printf("[%d]: %p\n", i, &elm[i]);
	}

	list_init(&list);

	e = list_pop_front(&list);
	if (e != NULL) {
		printf("%p\n", e);
	}

	e = list_pop_back(&list);
	if (e != NULL) {
		printf("%p\n", e);
	}

	list_push_front(&list, &elm[0]);
	list_push_back(&list, &elm[1]);
	list_push_back(&list, &elm[2]);
	list_push_front(&list, &elm[3]);

	for (e = list.root.next; e != &list.root; e = e->next) {
		printf("%p\n", e);
	}

	/* reverse */
	for (e = list.root.prev; e != &list.root; e = e->prev) {
		printf("%p\n", e);
	}

	e = list_pop_front(&list);
	if (e != NULL) {
		printf("pop front: %p\n", e);
	}

	for (e = list.root.next; e != &list.root; e = e->next) {
		printf("%p\n", e);
	}

	/* reverse */
	for (e = list.root.prev; e != &list.root; e = e->prev) {
		printf("%p\n", e);
	}

	e = list_pop_back(&list);
	if (e != NULL) {
		printf("pop back: %p\n", e);
	}

	for (e = list.root.next; e != &list.root; e = e->next) {
		printf("%p\n", e);
	}

	/* reverse */
	for (e = list.root.prev; e != &list.root; e = e->prev) {
		printf("%p\n", e);
	}

	list_pop_back(&list);
	list_pop_back(&list);

	for (e = list.root.next; e != &list.root; e = e->next) {
		printf("%p\n", e);
	}

	/* reverse */
	for (e = list.root.prev; e != &list.root; e = e->prev) {
		printf("%p\n", e);
	}
}

int
main(void)
{
	test1();
	return 0;
}
