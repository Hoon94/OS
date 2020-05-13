#include "Shell.h"
char* Getenv(int argc, char* argv[])
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
