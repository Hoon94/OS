#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main()
{
	char *string;
	size_t buffersize = 50;
	string = (char *)malloc(buffersize * sizeof(char));
	string = "Hello world!\n";
	*(string + (strlen(string) - 1))= '\0';
	printf("%s", string);
	return;
}
