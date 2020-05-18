#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char * argv[])
{
	struct stat fstat_buf;
	int i;
	int ret;
	
	printf("argc : %d\n", argc);
	if (argc == 1) {
		printf("usage: stat file_path ... \n");
		return 0;
	} else {
		for (i = 0 ; i < argc-1 ; i++) {
			ret = stat(argv[i+1], &fstat_buf);
			//printf("buf : %s\n", argv);
			if (ret == -1) {
				perror("stat");
			}
		}
	}
	printf("argc %d\n", argc);
	return 0;
}
