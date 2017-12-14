
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t *threads;
int compteur = 0;


void * threadEnAttente(void * arg){
    pthread_mutex_lock(&mutex);
    printf("Je suis déverouillé ! Je suis le thread N°%d\n",(int)arg);
    pthread_mutex_unlock(&mutex);
    pthread_exit(3);

}


int main(int argc, char** argv) {
    int n = 50;
    pthread_mutex_lock(&mutex);
    threads = malloc(sizeof(pthread_t)*n);
    for (int i = 0; i < n; ++i) {
        pthread_create(&threads[i], NULL, threadEnAttente, i);
    }
    pthread_mutex_unlock(&mutex);
    for (int i = 0; i < n; ++i) {
        pthread_join(threads[i],NULL);
    }



}