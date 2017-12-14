
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t mutexRDVa = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexRDVb = PTHREAD_MUTEX_INITIALIZER;

pthread_t th1;
pthread_t th2;
pthread_t th3;



void * T1(void * arg){
    printf("Je suis T1, j'attends le début du RDV\n");
    sleep(3);
    pthread_mutex_unlock(&mutexRDVa);
    pthread_mutex_lock(&mutexRDVb);
    puts("Le rendez vous a commencé !");
    pthread_exit(3);

}
void * T2(void * arg){
    printf("Je suis T2, j'attends le début du RDV\n");
    sleep(5);
    pthread_mutex_unlock(&mutexRDVb);
    pthread_mutex_lock(&mutexRDVa);
    puts("Le rendez vous a commencé !");
    pthread_exit(3);


}


int main(int argc, char** argv) {

    pthread_mutex_lock(&mutexRDVa);
    pthread_mutex_lock(&mutexRDVb);

    pthread_create(&th1, NULL, T1, NULL);
    pthread_create(&th2, NULL, T2, NULL);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);


}