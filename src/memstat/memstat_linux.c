#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "memstat.h"


int
memstat_get(Memstat *m)
{
	FILE *fp;
	char line[80];
	char *p;
	char *end;
	size_t drop;
	size_t *list[] = {
		/* size */
		&m->vs,
		/* resident */
		&m->rs,
		/* share */
		&drop,
		/* text */
		&m->text,
		/* lib */
		&drop,
		/* data + stack */
		&m->data,
		/* dt */
		&drop,
		NULL,
	};
	size_t **v;

	m->vs = 0;
	m->rs = 0;
	m->text = 0;
	m->data = 0;
	m->stack = 0;

	fp = fopen("/proc/self/statm", "r");
	if (fp == NULL)
		return -1;

	if (fgets(line, sizeof(line), fp) == NULL) {
		fclose(fp);
		return -1;
	}
	fclose(fp);

	p = line;
	for (v = list; *v != NULL; v++) {
		**v = strtoul(p, &end, 10);
		if (p == end)
			break;
		if (*end == '\0')
			break;
		p = end;
	}

	return 0;
}
