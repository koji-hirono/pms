#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/user.h>

#include <unistd.h>

#include "memstat.h"


int
memstat_get(Memstat *m)
{
	int mib[4];
	size_t len;
	struct kinfo_proc kp;

	m->vs = 0;
	m->rs = 0;
	m->text = 0;
	m->data = 0;
	m->stack = 0;

	mib[0] = CTL_KERN;
	mib[1] = KERN_PROC;
	mib[2] = KERN_PROC_PID;
	mib[3] = getpid();

	len = sizeof(kp);
	if (sysctl(mib, 4, &kp, &len, NULL, 0) == -1)
		return -1;

	m->vs = kp.ki_size / sysconf(_SC_PAGESIZE);
	m->rs = kp.ki_rssize;
	m->text = kp.ki_tsize;
	m->data = kp.ki_dsize;
	m->stack = kp.ki_ssize;

	return 0;
}
