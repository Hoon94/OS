#include "Shell.h"
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
