#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define ASCII_SIZE	256
int stat [ASCII_SIZE];
void char_stat(char* argv);
int Ncons;
//int flag;

typedef struct buffer {
	char *line;
	int full;
	int linenum;
	int count;
	pthread_mutex_t lock;
	pthread_cond_t cond;
}buffer;

typedef struct sharedobject {
	FILE *rfile;
	buffer buf[100];
	int flag;
}so_t;

void *producer(void *arg) {
	so_t *so = arg;
	int *ret = malloc(sizeof(int));
	FILE *rfile = so->rfile;
	int i = 0;
	int j = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;

	while (1) {
		read = getdelim(&line, &len, '\n', rfile);
		pthread_mutex_lock(&so->buf[j].lock);
		while(so->buf[j].full == 1)
		{
			pthread_cond_wait(&so->buf[j].cond, &so->buf[j].lock);
		}
		if (read == -1) 
		{
			for(int k=0; k<Ncons; k++){
                        	if(so->buf[k].full==0)
				{
                                        so->buf[k].count = 1;
                        	}
                	}
			so->flag = 1;
			pthread_cond_signal(&so->buf[j].cond);
			pthread_mutex_unlock(&so->buf[j].lock);
			j = (j + 1) % Ncons;
			break;
		}
		so->buf[j].linenum = i;
		so->buf[j].line = strdup(line);      /* share the line */
		so->buf[j].full = 1;
		i++;
		pthread_cond_signal(&so->buf[j].cond);
		pthread_mutex_unlock(&so->buf[j].lock);
		j = (j + 1) % Ncons;
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
	int j = 0;
	int len;
	char *line;

	while (1) {
		pthread_mutex_lock(&so->buf[j].lock);
		if(so->flag == 0)
		{
			while(so->buf[j].full == 0)
			{
				//int temp = j;
				//j = (j + 1) % Ncons;
				//pthread_mutex_unlock(&so->buf[temp].lock);
				pthread_cond_wait(&so->buf[j].cond, &so->buf[j].lock);
				//pthread_mutex_lock(&so->buf[j].lock);
				if(so->flag == 1)break;
			}
			if(so->flag == 1){
				pthread_mutex_unlock(&so->buf[j].lock);
				//continue;
			}
			if(so->buf[j].line != NULL)//&&(so->buf[j].full == 1))
			{
				line = so->buf[j].line;
				char_stat(line);
				printf("Cons_%x: [%02d:%02d] %s",
					(unsigned int)pthread_self(), i, so->buf[j].linenum, line);
				free(so->buf[j].line);
				so->buf[j].line = NULL;
				i++;
				so->buf[j].full = 0;
				pthread_cond_signal(&so->buf[j].cond);
				pthread_mutex_unlock(&so->buf[j].lock);
				j = (j + 1) % Ncons;
			}
		}
		else
		{
			line = so->buf[j].line;
			if ((line == NULL)||(so->buf[j].count == 1)){
				pthread_cond_signal(&so->buf[j].cond);
				pthread_mutex_unlock(&so->buf[j].lock);
				j = (j + 1) % Ncons;
				break;
			}
			char_stat(line);
			printf("Cons_%x: [%02d:%02d] %s",
				(unsigned int)pthread_self(), i, so->buf[j].linenum, line);
			free(so->buf[j].line);
			so->buf[j].line = NULL;
			so->buf[j].count = 1;
			i++;
			pthread_cond_signal(&so->buf[j].cond);
			pthread_mutex_unlock(&so->buf[j].lock);
			j = (j + 1) % Ncons;
		}
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
	int Nprod;
	int rc;   long t;
	int *ret;
	int i;
	clock_t start, finish;
	double duration;
	FILE *rfile;
	if (argc == 1) {
		printf("usage: ./prod_cons <readfile> #Producer #Consumer\n");
		exit (0);
	}
	buffer *buf = malloc(sizeof(buffer));
	memset(buf, 0, sizeof(buffer));
	so_t *share = malloc(sizeof(so_t));
	memset(share, 0, sizeof(so_t));
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

	share->rfile = rfile;
	
	for(i=0;i<Ncons;i++){
	share->buf[i].line = NULL;
	pthread_mutex_init(&share->buf[i].lock, NULL);
	pthread_cond_init(&share->buf[i].cond, NULL);
	}
	start = clock();
	for (i = 0 ; i < Nprod ; i++)
		pthread_create(&prod[i], NULL, producer, share);
	for (i = 0 ; i < Ncons ; i++)
		pthread_create(&cons[i], NULL, consumer, share);
	printf("main continuing\n");

	for (i = 0 ; i < Ncons ; i++) {
		rc = pthread_join(cons[i], (void **) &ret);
		printf("main: consumer_%d joined with %d\n", i, *ret);
	}
	for (i = 0 ; i < Nprod ; i++) {
		rc = pthread_join(prod[i], (void **) &ret);
		printf("main: producer_%d joined with %d\n", i, *ret);
	}
	finish = clock();
	duration = (float)(finish-start);
	printf("       A        B        C        D        E        F        G        H        I        J        K        L        M        N        O        P        Q        R        S        T        U        V        W        X        Y        Z\n");
        printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\n",
                        stat['A']+stat['a'], stat['B']+stat['b'],  stat['C']+stat['c'],  stat['D']+stat['d'],  stat['E']+stat['e'],
                        stat['F']+stat['f'], stat['G']+stat['g'],  stat['H']+stat['h'],  stat['I']+stat['i'],  stat['J']+stat['j'],
                        stat['K']+stat['k'], stat['L']+stat['l'],  stat['M']+stat['m'],  stat['N']+stat['n'],  stat['O']+stat['o'],
                        stat['P']+stat['p'], stat['Q']+stat['q'],  stat['R']+stat['r'],  stat['S']+stat['s'],  stat['T']+stat['t'],
                        stat['U']+stat['u'], stat['V']+stat['v'],  stat['W']+stat['w'],  stat['X']+stat['x'],  stat['Y']+stat['y'],
                        stat['Z']+stat['z']);
	printf("%fms\n", duration);
	pthread_exit(NULL);
	exit(0);
}
