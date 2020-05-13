#include "Shell.h"
void CD(int argc, char* argv)
{
        char *path;
        if(argc > 1)
        {
                path = argv;
        }
        //else if((path = (char*)getenv("HOME")) == NULL)
        //{
        //        path = ".";
        //}
        if(chdir(path) < 0)
        {
                printf("error : No directory\n");
        }
}
