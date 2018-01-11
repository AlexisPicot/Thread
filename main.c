#include <stdio.h>
#include <unistd.h>
#include "Djikstra.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <ctype.h>

#define SEM_FINISH        6
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
    int indice;
    pthread_mutex_t mutex;
    char const **argv;
} mutex;

mutex  indice=  {
    .indice = 2,
    .mutex=PTHREAD_MUTEX_INITIALIZER
};


int argc;
char const ***argv;
pthread_t *threads;
unsigned char *image;


void * thread(void * arg){
    int x;
    do {
        pthread_mutex_lock(&indice.mutex);
        x = indice.indice++;
        pthread_mutex_unlock(&indice.mutex);
        if(x>=argc-1) break;

        FILE* fp1, *fp2;
        int c;
        fp1= fopen (indice.argv[x], "r");
        fp2= fopen (indice.argv[x], "r+");
        if ((fp1== NULL) || (fp2== NULL)) {
            perror ("fopen"); exit (1);
        }
        while (c !=EOF) {
            c=fgetc(fp1);
            if (c!=EOF) fputc(toupper(c),fp2);
        }
        fclose (fp1); fclose (fp2);


    }while (1);
        V(sem_create(SEM_FINISH, NULL));
}
int main(int _argc, char const *_argv[])
{
    argc=_argc;
    indice.argv=_argv;
    int nbThread = MIN(atoi(_argv[1]),argc);
    pthread_mutex_unlock(&indice.mutex);
    sem_delete(sem_create(SEM_FINISH,NULL));
    sem_create(SEM_FINISH,0);

    threads = malloc(sizeof(pthread_t)*nbThread);


    for (int y = 0; y < nbThread; ++y) {
       pthread_create(&threads[y], NULL, thread,0);
    }

    P(sem_create(SEM_FINISH,NULL));
    return 0;
}

