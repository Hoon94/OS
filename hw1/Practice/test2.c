#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#define MAX_LINE 256


int Stat(int argc, char* argv);
int Getenv(int argc, char* argv[]);
char *arr[50];

int main(int argc, char* argv[])
{
	char buf[MAX_LINE];
	char *item;
	char *token[100], *saveptr;
	int j;
	int i;
	pid_t pid;
	//char** aaa;
	//char* aaa[50];

	printf("My Shell\n");
	/*aaa = (char**)malloc(sizeof(char)*10);
	for(int i=0; i<10 ; i++)
	{	
		aaa[i] = (char*)malloc(sizeof(char)*10);
	}*/
	for(i = 0; i < 50; i++){
	arr[i] = (char*)malloc(sizeof(char)*MAX_LINE);
	}

	while(1)
	{
		memset(buf, 0x00, MAX_LINE);
		fgets(buf, MAX_LINE, stdin);
		item = buf;
		//*(item + (strlen(item) - 1)) = '\0';
		//for(int k = 0; k < sizeof(aaa); k++)
		//{
		//	memset(aaa[k], NULL, sizeof(aaa));
		//}
		for(j = 0; ;item = NULL, j++)
		{
			token[j] = strtok_r(item, " ", &saveptr);
			if(token[j] == NULL)
			{
				//for(j; j < strlen(*aaa); j++)
				//{
				//	aaa[j] = NULL;
					break;
				//}
			}
			//strcpy(aaa[j],token[j]);
			//*(arr[j] + (strlen(arr[j]) - 1)) = '\0';
		}

		if(strncmp(buf,"quit\n", 5) == 0)
		{
			exit(0);
		}

		//(aaa + (strlen(*aaa) - 1)) = '\0';
		if(Getenv(j + 1, token))
		{
			pid = fork();
			if(pid == 0)
			{
				if(execve(arr[0], arr, NULL) == -1)
				{	printf("aaa : %s\n", arr[0]);
					printf("Execve : %d\n", execve(arr[0], arr, NULL));
					printf("Execve failure\n");
					exit(0);
				}
				else
				{
					execve(arr[0], arr, NULL);
				}
			}
			else if(pid < 0)
			{
				perror("fork error");
				return 0;
			}
			else
			{
				printf("Parent wait\n");
				wait(0);
				printf("Child exit\n");
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
	printf("argc %d\n", argc);
	printf("argv %s\n", argv);
	if (argc == 1) {
		printf("usage: stat file_path ... \n");
		return 0;
	} else {
		for (i = 0 ; i < argc-1 ; i++) {
			char* new_argv = (char *)malloc(sizeof(int)*100);
			for(int a = 0; a < strlen(argv) - 1; a++)
			{
				new_argv[a]=argv[a];
			}
			printf("argv %s \n",argv);
			ret = stat(new_argv, &fstat_buf);
			printf("ret : %d\n", ret);
			if (ret == -1) {
				perror("stat");
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
	//char *arr;

	//arr = (char*)malloc(sizeof(char)*100);

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
			printf("%s=%s\n", argv[i], env);
			for (j=0,str=env; ;str= NULL,j++)
			{
				tok[j] = strtok_r(str, ":", &saveptr);
				if (tok[j] == NULL) break;
				printf("\t%s\n", tok[j]);
				
			}
			printf("***---------------------***\n");
		}
	}
	for(k; k < j-1; k++)
	{
		strcpy(arr, tok[k]);
		strcat(arr, "/");
		printf("%s\n", arr);
		//for(l=0; l < argc-1; l++)
		//{
		strcat(arr, argv[l]);
		printf("%s\n", arr);
		//}
		if(Stat(argc, arr))
		{
			return 1;
		}
	}

	return 0;
}
