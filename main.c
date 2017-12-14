
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t ThreadA;
pthread_t ThreadB;
int compteur = 0;


void * consommer(void * arg){
    while (compteur<=40){
        printf("%d",compteur);
        fflush(stdout);
        usleep(100000);
        sched_yield();
    }
    pthread_exit(3);

}
void * produire(void *arg){
    while (true){
        compteur++;
        usleep(100000);
        sched_yield();
    }
}

int main(int argc, char** argv) {

    printf("%s",argv[1]);
    pthread_create(&ThreadB,NULL,consommer,NULL);
    pthread_create(&ThreadA, NULL, produire, NULL);
    pthread_join(ThreadA,NULL);
    pthread_cancel(ThreadB);
}