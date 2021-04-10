#ifndef SUBPROC_H__
#define SUBPROC_H__

typedef struct Subproc Subproc;

struct Subproc {
	int pid;
	int in;
	int out;
	int err;
};

#define P_IN  (1 << 0)
#define P_OUT (1 << 1)
#define P_ERR (1 << 2)

extern int subproc_vopen(Subproc *, char *const *, int);
extern void subproc_close(Subproc *);
extern void subproc_wait(Subproc *);

#endif
