#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#define MAX_LINE 256

int Stat(int argc, char* argv);
char *Getenv(int argc, char* argv[]);
void CD(int argc, char* argv);

int main(int argc, char* argv[])
{
	char buf[MAX_LINE];
	char buf1[255];
	char *item;
	char *token[100], *saveptr;
	int i, j;
	int res;
	char *arr = (char*)malloc(sizeof(char)*100);
	pid_t pid;
	getcwd(buf1, 255);
	while(1)
	{
		printf("SiSH:%s$ ", buf1);
		memset(buf, 0x00, MAX_LINE);
		fgets(buf, MAX_LINE, stdin);
		item = buf;
		item[strlen(item)-1]='\0';
		for(j = 0; ;item = NULL, j++)
		{
			token[j] = strtok_r(item, " ", &saveptr);
			if(token[j] == NULL)
			{
				break;
			}	
		}
		if(strncmp(buf,"quit", 4) == 0)
		{
			exit(0);
		}
		if(!strcmp("cd", token[0]))
		{
			CD(2, token[1]);
		}
		else
		{	
			arr = Getenv(2, &token[0]);
			if(!(arr == 0)){
			pid = fork();
			if(pid == 0)
			{	
				int res = execve(arr, token, NULL);
				if(res == -1)
				{
					printf("Execve failure\n");
				}
				exit(0);
			}
			else if(pid < 0)
			{
				perror("fork error");
				return 0;
			}
			else
			{
				wait(0);
			}}
		}
	}
	return 0;
}

void CD(int argc, char* argv)
{
	char *path;
	if(argc > 1)
	{
		path = argv;
	}
	else if((path = (char*)getenv("HOME")) == NULL)
	{
		path = ".";
	}
	if(chdir(path) < 0)
	{
		printf("error : No directory\n");
	}
}

int Stat(int argc, char* argv)
{
	struct stat fstat_buf;
	int i;
	int ret;
	if (argc == 1) 
	{
		printf("usage: stat file_path ... \n");
		return 0;
	}
	else
	{
		for (i = 0 ; i < argc-1 ; i++)
		{
			ret = stat(argv, &fstat_buf);
			if (ret == -1) 
			{
				return 0;
			}			
			return 1;
		}
	}
}	

char *Getenv(int argc, char* argv[])
{
	int i, j=0;
	int k = 0;
	char *env, *str;
	char *tok[100], *saveptr;
	char copy[MAX_LINE];
	char *p;
	time_t timer;
	p = argv[0];
	char *arr = (char*)malloc(sizeof(char)*100);

	if(argc == 1)
	{
		printf("usage: getenv env_vars ... \n");
		return 0;
	}
	else
	{
		for (i = 0 ; i < argc-1 ; i++)
		{
			env = getenv("PATH");
			strcpy(copy, env);
			for (j=0, str = copy; ;str = NULL, j++)
			{
				tok[j] = strtok_r(str, ":", &saveptr);
				if (tok[j] == NULL) 
				{
					break;
				}
			}
		}
	}
	if(isupper(*p))
	{
		if((strncmp(*argv, "TIME", 4))==0)
		{
			time(&timer);
			setenv("TIME", ctime(&timer), 1);
		}	
		env = getenv(*argv);
		printf("%s=%s\n", *argv, env);
	}
	else
	{
		for(k; k < j-1; k++)
		{
			if((strncmp(tok[k], *argv, 4))==0)
			{
				if(Stat(argc, *argv))
				{
					strcpy(arr, *argv);
					return arr;
				}
			}
			strcpy(arr, tok[k]);
			strcat(arr, "/");
			strcat(arr, *argv);
			if(Stat(argc, arr))
			{
				return arr;
			}
		}
		printf("%s: command not found\n", *argv);
	}
	return 0;
}
