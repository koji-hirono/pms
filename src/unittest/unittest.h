#ifndef UNITTEST_H__
#define UNITTEST_H__

typedef struct Utcase Utcase;
typedef struct Ut Ut;

struct Utcase {
	const char *name;
	void (*exec)(Ut *);
};

struct Ut {
	Utcase *cur;
	int flags;
	int tested;
	int failed;
};

#define UT_VERBOSE (1 << 0)
#define UT_STOP    (1 << 1)

#define ut_assert(ut, expr) \
	ut_assert_exec(ut, expr, __FILE__, __LINE__, #expr)

extern void ut_init(Ut *);
extern void ut_run(Ut *, Utcase **);
extern void ut_assert_exec(Ut *, int, const char *, int, const char *);

#endif
