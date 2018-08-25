#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "unittest.h"


static void
test1_func(Ut *ut)
{
	ut_assert(ut, 1 == 0);
	ut_assert(ut, 0 == 1);
}

static Utcase test1_case = {
	"Sample Test #1",
	test1_func
};

static void
test2_func(Ut *ut)
{
	ut_assert(ut, 2 > 1);
	ut_assert(ut, 2 < 0);
}

static Utcase test2_case = {
	"Sample Test #2",
	test2_func
};


int
main(int argc, char **argv)
{
	int opt;
	Ut ut;
	Utcase *ut_list[] = {
		&test1_case,
		&test2_case,
		NULL
	};

	ut_init(&ut);
	while ((opt = getopt(argc, argv, "sv")) != -1) {
		switch (opt) {
		case 's':
			ut.flags |= UT_STOP;
			break;
		case 'v':
			ut.flags |= UT_VERBOSE;
			break;
		default:
			return EXIT_FAILURE;
		}
	}

	ut_run(&ut, ut_list);

	return EXIT_SUCCESS;
}
