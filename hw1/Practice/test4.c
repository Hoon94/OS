#include <stdio.h>
#include <unistd.h>

int main()
{
	char buf[255];
	getcwd(buf, 255);
	printf("%s\n", buf);
}
