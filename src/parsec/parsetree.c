#include <stdlib.h>
#include <string.h>

#include "parsetree.h"


ParseNode *
parsetree_list_create(void)
{
	ParseNode *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return NULL;
	node->type = 'L';
	node->list.n = 0;
	node->list.data = NULL;
	return node;
}

ParseNode *
parsetree_str_create(const char *s, size_t len)
{
	ParseNode *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return NULL;
	node->type = 'S';
	node->str.s = (char *)s;
	node->str.len = len;
	return node;
}

void
parsetree_free(ParseNode *node)
{
	int i;

	if (node == NULL)
		return;

	switch (node->type) {
	case 'L':
		for (i = 0; i < node->list.n; i++) {
			parsetree_free(node->list.data[i]);
		}
		free(node->list.data);
		break;
	default:
		break;
	}

	free(node);
}

int
parsetree_list_append(ParseNode *node, ParseNode *child)
{
	ParseNode **data;
	int n;

	n = node->list.n + 1;
	data = realloc(node->list.data, sizeof(node) * n);
	if (data == NULL)
		return -1;

	node->list.data = data;
	node->list.data[node->list.n++] = child;

	return 0;
}
