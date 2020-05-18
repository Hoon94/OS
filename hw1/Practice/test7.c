#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	time_t timer;
	time(&timer);

	printf("time : %s", ctime(&timer));
	return 0;
}	
