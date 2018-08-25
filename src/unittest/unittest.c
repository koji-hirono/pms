#include <stdio.h>
#include <stdlib.h>

#include "unittest.h"


void
ut_init(Ut *ut)
{
	ut->cur = NULL;
	ut->flags = 0;
	ut->tested = 0;
	ut->failed = 0;
}

void
ut_run(Ut *ut, Utcase **top)
{
	Utcase **c;

	for (c = top; *c != NULL; c++) {
		ut->cur = *c;
		ut->cur->exec(ut);
	}

	if (ut->flags & UT_VERBOSE) {
		printf("%d tests, %d failures\n", ut->tested, ut->failed);
	} else {
		printf("\n%d/%d passed\n", ut->tested - ut->failed, ut->tested);
	}
}

void
ut_assert_exec(Ut *ut, int r, const char *file, int line, const char *msg)
{
	Utcase *c = ut->cur;

	if (ut->flags & UT_VERBOSE) {
		printf("%s ... ", c->name);
		fflush(stdout);
	}

	ut->tested++;
	if (r) {
		if (ut->flags & UT_VERBOSE) {
			printf("pass\n");
		} else {
			printf(".");
			fflush(stdout);
		}
	} else {
		ut->failed++;
		if (ut->flags & UT_VERBOSE) {
			printf("fail\n");
			printf("%s(%d): %s\n", file, line, msg);
		} else {
			printf("F");
			fflush(stdout);
		}

		if (ut->flags & UT_STOP) {
			abort();
		}
	}
}
