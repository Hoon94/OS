#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_STRING_LENGTH 30
#define ASCII_SIZE      256
int stat [MAX_STRING_LENGTH];
int stat2 [ASCII_SIZE];
int char_stat(int argc, char *argv[]);

typedef struct sharedobject {
        FILE *rfile;
        int linenum;
        char *line[10];
        pthread_mutex_t lock;
        pthread_cond_t cond;
        //int full[5];
        int in;
        int out;
        int count;
} so_t;

void *producer(int *arg);
void *consumer(int *arg);
