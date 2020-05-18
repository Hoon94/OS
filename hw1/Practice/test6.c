#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#define MAX_LINE 256


int Stat(int argc, char* argv);
int Getenv(int argc, char* argv[]);
char *arr;

int main(int argc, char* argv[])
{
	char buf[MAX_LINE];
	char buf1[255];
	char *item;
	char *token[100], *saveptr;
	int i, j;
	int res;
	pid_t pid;

	getcwd(buf1, 255);

	while(1)
	{
		printf("%s$ ", buf1);
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

		if(Getenv(2, &token[0]))
		{

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
			}
		}

	}
	return 0;
}

int 
Stat(int argc, char* argv)
{
	struct stat fstat_buf;
	int i;
	int ret;
	if (argc == 1) {
		printf("usage: stat file_path ... \n");
		return 0;
	} else {
		for (i = 0 ; i < argc-1 ; i++) {
			ret = stat(argv, &fstat_buf);
			if (ret == -1) 
			{
				//perror("stat");
				return 0;
			}			
			return 1;
		}
	}
}	

int
Getenv(int argc, char* argv[])
{
	int i, j=0;
	int k = 0;
	int l;
	char *env, *str;
	char *tok[100], *saveptr;
	char *p;

	p = argv[0];
	arr = (char*)malloc(sizeof(char)*100);

	if(argc == 1)
	{
		printf("usage: getenv env_vars ... \n");
		return 0;
	}
	else
	{
		for (i = 0 ; i < argc-1 ; i++){
			env = getenv("PATH");
			char copy[256];
			strcpy(copy, env);
			//printf("%s=%s\n", argv[0], copy);
			for (j=0, str = copy; ;str = NULL, j++)
			{
				tok[j] = strtok_r(str, ":", &saveptr);
				if (tok[j] == NULL) 
				{
					break;
				}
				//printf("\t%s\n", tok[j]);
				
			}
			//printf("***---------------------***\n");
		}
	}
	
	if(isupper(*p))
	{	
		env = getenv(argv[0]);
		printf("%s=%s\n", argv[0], env);
	}
	else{
		for(k; k < j-1; k++)
		{
			strcpy(arr, tok[k]);
			strcat(arr, "/");
			strcat(arr, *argv);
			if(Stat(argc, arr))
			{
				return 1;
			}
		}
	}
	return 0;
}
