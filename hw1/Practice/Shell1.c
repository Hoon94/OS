#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int 
main(int argc, char * argv[])
{

	char **new_argv;
	char command[] = "ls";
	int idx;

	new_argv = (char **)malloc(sizeof(char *)*(argc + 1));

	new_argv[0] = command;

	for(idx = 0;idx < argc;idx++)
	{
		new_argv[idx] = argv[idx];
	}

	new_argv[argc] = NULL;

	if(execve("/usr/bin/ls", new_argv, environ) == -1)
	{
		fprintf(stderr, "Error : %s\n", strerror(errno));
		return 1;
	}

	printf("_________________________________\n");

	return 0;
}







/*int
stat()
{

	struct stat fstat_buf;
	int i;
	int ret;
	if (argc == 1)
	{
		printf("usage : stat file_path ... \n");
	}
	else
	{
		for(i=0;i<argc-1;i++)
		{
			ret = stat(argv[i+1], &fstat_buf);
			if(ret == -1)
			{
				perror("stat");
			}
		}
	}


	return 0;
}*/
