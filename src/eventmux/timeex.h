#ifndef TIMEEX_H__
#define TIMEEX_H__

#define timespec_init(t, sec, nsec) do { \
	(t)->tv_sec = (sec); \
	(t)->tv_nsec = (nsec); \
} while (0)

#define timespec_iszero(t) \
	((t)->tv_sec == 0 && (t)->tv_nsec == 0)

#define timespec_cmp(a, b, cmp) \
	(((a)->tv_sec == (b)->tv_sec) ? \
	  ((a)->tv_nsec cmp (b)->tv_nsec) : \
	  ((a)->tv_sec cmp (b)->tv_sec))

#define timespec_add(dst, a, b) do { \
	(dst)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
	(dst)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec; \
	if ((dst)->tv_nsec >= 1000000000) { \
		(dst)->tv_sec++; \
		(dst)->tv_nsec -= 1000000000; \
	} \
} while (0)

#define timespec_sub(dst, a, b) do { \
	(dst)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
	(dst)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec; \
	if ((dst)->tv_nsec < 0) { \
		(dst)->tv_sec--; \
		(dst)->tv_nsec += 1000000000; \
	} \
} while (0)

#endif
