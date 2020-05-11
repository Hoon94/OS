#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

int sum = 0;
int main(int argc, char * argv[])
{
	int number = 0; 
	int i = 0;
	if (argc == 2) {
		number = atoi(argv[1]);
		if ((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN))
			|| (errno != 0 && number == 0)) {
               perror("strtol");
               exit(EXIT_FAILURE);
		}
	} else  {
		number = 100;
	}

	for (i = 0 ; i <= number ; i++) {
		sum += i;
	}
	printf("sum from 0 to %d: %d (0x%x)\n", number, sum, sum);
	return 0;
}
