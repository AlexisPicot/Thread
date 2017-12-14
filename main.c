
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t th1;
pthread_t th2;



void * T1(void * arg){
    pthread_mutex_lock(&mutex);
    puts("Je suis T1, je fais mon traitement");
    sleep(3);
    puts("Je suis T1, j'ai fini");
    pthread_mutex_unlock(&mutex);
    pthread_exit(3);

}
void * T2(void * arg){
    puts("Je suis T2, j'attends que T1 ait fini");
    pthread_mutex_lock(&mutex);
    puts("Je suis T2, je suis déverouillé ! Je fais mon traitement");
    sleep(3);
    puts("Je suis T2, j'ai fini");
    pthread_exit(3);

}


int main(int argc, char** argv) {

    pthread_create(&th1, NULL, T1, 0);
    pthread_create(&th2, NULL, T2, 0);
    pthread_mutex_unlock(&mutex);


    pthread_join(th1,NULL);
    pthread_join(th2,NULL);


}