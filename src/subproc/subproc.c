#include <sys/types.h>
#include <sys/wait.h>

#include <stddef.h>
#include <signal.h>

#include <unistd.h>

#include "subproc.h"


int
subproc_vopen(Subproc *s, char *const *argv, int flags)
{
	int in[2];
	int out[2];
	int err[2];

	signal(SIGCHLD, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);

	if (flags & P_IN)
		if (pipe(in) != 0)
			return -1;
	if (flags & P_OUT) {
		if (pipe(out) != 0) {
			if (flags & P_IN) {
				close(in[0]);
				close(in[1]);
			}
			return -1;
		}
	}
	if (flags & P_ERR) {
		if (pipe(err) != 0) {
			if (flags & P_IN) {
				close(in[0]);
				close(in[1]);
			}
			if (flags & P_OUT) {
				close(out[0]);
				close(out[1]);
			}
			return -1;
		}
	}

	s->pid = fork();
	if (s->pid == -1) {
		if (flags & P_IN) {
			close(in[0]);
			close(in[1]);
		}
		if (flags & P_OUT) {
			close(out[0]);
			close(out[1]);
		}
		if (flags & P_ERR) {
			close(err[0]);
			close(err[1]);
		}
		return -1;
	}

	if (s->pid == 0) {
		if (flags & P_IN) {
			close(in[1]);
			dup2(in[0], 0);
		}
		if (flags & P_OUT) {
			close(out[0]);
			dup2(out[1], 1);
		}
		if (flags & P_ERR) {
			close(err[0]);
			dup2(err[1], 2);
		}
		execv(argv[0], argv);
		_exit(127);
	}

	if (flags & P_IN) {
		close(in[0]);
		s->in = in[1];
	} else {
		s->in = -1;
	}
	if (flags & P_OUT) {
		close(out[1]);
		s->out = out[0];
	} else {
		s->out = -1;
	}
	if (flags & P_ERR) {
		close(err[1]);
		s->err = err[0];
	} else {
		s->err = -1;
	}

	return 0;
}

void
subproc_close(Subproc *s)
{
	int status;
	int ret;

	if (s->in != -1)
		close(s->in);
	if (s->out != -1)
		close(s->out);
	if (s->err != -1)
		close(s->err);

	ret = waitpid(s->pid, &status, WNOHANG);
	if (ret == 0) {
		kill(s->pid, SIGTERM);
		sleep(1);
		ret = waitpid(s->pid, &status, WNOHANG);
		if (ret == 0) {
			kill(s->pid, SIGKILL);
			sleep(1);
			waitpid(s->pid, &status, WNOHANG);
		}
	}
}

void
subproc_wait(Subproc *s)
{
	int status;

	waitpid(s->pid, &status, 0);
}
