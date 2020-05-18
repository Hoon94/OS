#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

void sig_usr(int signum)
{
	printf("Got signal %d\n", signum);
	exit(0);
}

int main(int argc, char *argv[])
{
	pid_t cpid;
	signal(SIGUSR1, sig_usr);

	cpid = fork();
	if(cpid == 0)
	{
		printf("sending sig to parent\n");
		kill(getppid(), SIGUSR1);
		exit(0);
	}
	else if(cpid > 0)
	{
		wait(0);
	}
	else
	{
		perror("fork");
	}
}
