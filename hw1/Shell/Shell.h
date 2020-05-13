#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#define MAX_LINE 256

int Stat(int argc, char* argv);
char* Getenv(int argc, char* argv[]);
void CD(int argc, char* argv);
