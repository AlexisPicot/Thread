
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "djikstra.h"

pthread_t th1;
pthread_t th2;
pthread_t th3;



void * T1(void * arg){
    printf("Je suis T1, j'attends le début du RDV\n");
    sleep(3);
    P(sem_create(1, NULL));
    puts("Le rendez vous a commencé !");
    pthread_exit(3);

}
void * T2(void * arg){
    printf("Je suis T2, j'attends le début du RDV\n");
    sleep(5);
    P(sem_create(1, NULL));
    puts("Le rendez vous a commencé !");
    pthread_exit(3);


}void * T3(void * arg){
    printf("Je suis T3, je suis le dernier arrivé, début du RDV\n");
    sleep(5);
    V(sem_create(1, NULL));
    V(sem_create(1, NULL));
    puts("Le rendez vous a commencé !");
    pthread_exit(3);


}


int main(int argc, char** argv) {

    sem_create(1, 0);


    pthread_create(&th1, NULL, T1, NULL);
    pthread_create(&th2, NULL, T2, NULL);
    pthread_create(&th3, NULL, T3, NULL);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);


}