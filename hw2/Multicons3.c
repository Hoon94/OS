#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define MAX_STRING_LENGTH 30
#define ASCII_SIZE	256
int stat [ASCII_SIZE];
void char_stat(char* argv);
FILE *rfile;
int End = 0;

typedef struct sharedobject {
	int linenum;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	char *line;
	char full;
	int in;
	int count;
}so_t;

void *producer(void *arg) {
	so_t *so = arg;
	int *ret = malloc(sizeof(int));
	int i = 0; 
	int j = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;

	while (1) {
		read = getdelim(&line, &len, '\n', rfile);
		pthread_mutex_lock(&so[so->in].lock);
		while(so->full == 1)
		{
			pthread_cond_wait(&so->cond, &so->lock);
		}
		if (read == -1) {
			so->full = 1;
			so->line = NULL;
			so->count = 1;
			End = 1;
			pthread_cond_signal(&so->cond);
			pthread_mutex_unlock(&so->lock);
			break;
		}
		so->linenum = i;
		so->line = strdup(line);      /* share the line */
		so->full = 1;
		i++;
		pthread_cond_signal(&so->cond);
		pthread_mutex_unlock(&so->lock);
	}
	free(line);
	line = NULL;
	printf("Prod_%x: %d lines\n", (unsigned int)pthread_self(), i);
	*ret = i;
	pthread_exit(ret);
}

void *consumer(void *arg) {
	so_t *so = arg;
	int *ret = malloc(sizeof(int));
	int i = 0;
	int len;
	char *line;
	while (1) {
		pthread_mutex_lock(&so->lock);
		while(so->full == 0)
		{
			pthread_cond_wait(&so->cond, &so->lock);
		}
		line = so->line;
		if (so->line == NULL) {
			so->full = 0;
			pthread_cond_signal(&so->cond);
			pthread_mutex_unlock(&so->lock);
			break;
		}
		char_stat(line);
		len = strlen(line);
		//printf("Cons_%x: [%02d:%02d] %s",
		//	(unsigned int)pthread_self(), i, so->linenum, line);
		free(so->line);
		so->line = NULL;
		line = NULL;
		i++;
		so->full = 0;
		pthread_cond_signal(&so->cond);
		pthread_mutex_unlock(&so->lock);
	}
	printf("Cons: %d lines\n", i);
	*ret = i;
	pthread_exit(ret);
}

void char_stat(char *argv)
{
        size_t length = 0;
	char *cptr = NULL;
        char *substr = NULL;
        char *brka = NULL;
        char *sep = "{}()[],;\" \n\t^";
        cptr = strdup(argv);
        for (substr = strtok_r(cptr, sep, &brka); substr; substr = strtok_r(NULL, sep, &brka))
        {
                length = strlen(substr);
         	for (int i = 0; i < length; i++)
		{
               		if (*substr < 256 && *substr > 1)
			{
                       	        stat[*substr]++;
			}
                substr++;
               	}
       	}
	free(cptr);
        return;
}

int main (int argc, char *argv[])
{
	pthread_t prod[100];
	pthread_t cons[100];
	int Nprod, Ncons;
	int rc;   long t;
	int *ret;
	int i;
	if (argc == 1) {
		printf("usage: ./prod_cons <readfile> #Producer #Consumer\n");
		exit (0);
	}
	so_t *share[100];
	for(int i = 0; i<100; i++){
	share[i] = malloc(sizeof(so_t));
	memset(share[i], 0, sizeof(so_t));}
	// initialize stat
	memset(stat, 0, sizeof(stat));
	rfile = fopen((char *) argv[1], "r");
	if (rfile == NULL) {
		perror("rfile");
		exit(0);
	}
	if (argv[2] != NULL) {
		Nprod = atoi(argv[2]);
		if (Nprod > 100) Nprod = 100;
		if (Nprod == 0) Nprod = 1;
	} else Nprod = 1;
	if (argv[3] != NULL) {
		Ncons = atoi(argv[3]);
		if (Ncons > 100) Ncons = 100;
		if (Ncons == 0) Ncons = 1;
	} else Ncons = 1;
	
	for(i=0;i<Ncons;i++){
	share[i]->line = NULL;
	pthread_mutex_init(&share[i]->lock, NULL);
	pthread_cond_init(&share[i]->cond, NULL);
	}
	for (i = 0 ; i < Nprod ; i++)
		pthread_create(&prod[i], NULL, producer, share[i]);
	for (i = 0 ; i < Ncons ; i++)
		pthread_create(&cons[i], NULL, consumer, share[i]);
	printf("main continuing\n");

	for (i = 0 ; i < Ncons ; i++) {
		rc = pthread_join(cons[i], (void **) &ret);
		printf("main: consumer_%d joined with %d\n", i, *ret);
	}
	for (i = 0 ; i < Nprod ; i++) {
		rc = pthread_join(prod[i], (void **) &ret);
		printf("main: producer_%d joined with %d\n", i, *ret);
	}
        printf(" A : %8d\nB : %8d\nC : %8d\nD : %8d\nE : %8d\nF : %8d\nG : %8d\nH : %8d\nI : %8d\nJ : %8d\nK : %8d\nL : %8d\nM : %8d\nN : %8d\nO : %8d\nP : %8d\nQ : %8d\nR : %8d\nS : %8d\nT : %8d\nU : %8d\nV : %8d\nW : %8d\nX : %8d\nY : %8d\nZ : %8d\n",
                        stat['A']+stat['a'], stat['B']+stat['b'],  stat['C']+stat['c'],  stat['D']+stat['d'],  stat['E']+stat['e'],
                        stat['F']+stat['f'], stat['G']+stat['g'],  stat['H']+stat['h'],  stat['I']+stat['i'],  stat['J']+stat['j'],
                        stat['K']+stat['k'], stat['L']+stat['l'],  stat['M']+stat['m'],  stat['N']+stat['n'],  stat['O']+stat['o'],
                        stat['P']+stat['p'], stat['Q']+stat['q'],  stat['R']+stat['r'],  stat['S']+stat['s'],  stat['T']+stat['t'],
                        stat['U']+stat['u'], stat['V']+stat['v'],  stat['W']+stat['w'],  stat['X']+stat['x'],  stat['Y']+stat['y'],
                        stat['Z']+stat['z']);

	pthread_exit(NULL);
	exit(0);
}
