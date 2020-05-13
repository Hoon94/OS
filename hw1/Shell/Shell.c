#include "Shell.h"
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
			arr = (Getenv(2, &token[0]));
                	if(!(arr == 0))
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
        }
        return 0;
}
