#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

int sum = 0;

int fib(int number);

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
		number = 10;
	}

	sum = fib(number);
	printf("%d-th Fibonacci number: %d (0x%x)\n", number, sum, sum);
	return 0;
}

int fib(int number)
{
	if (number < 3) return 1;
	else return (fib(number-1) + fib(number-2));
}
