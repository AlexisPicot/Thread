
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "djikstra.h"
#define SIZE_BUFFER 3
#define SEM_BUFFER 2

pthread_mutex_t mutexBufferEcriture = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexBufferLecture = PTHREAD_MUTEX_INITIALIZER;
//Strucure pour le buffer, cela permet de proteger une seule variable plutot que 3
typedef struct{
    int mem[SIZE_BUFFER];
    int *ptLecture;
    int *ptEcriture;
} BUFFER;


void * Prod(void * arg){
    puts("Je suis un producteur");
    while (true) {
        pthread_mutex_lock(&mutexBufferEcriture);
        //Je prend un jeton, cela permet d'éviter en gros que
        // l'écrivain ait plus de SIZE_BUFFER coup d'avance
        // sur le lecteur et donc que des données soient perdues
        P(sem_create(SEM_BUFFER, NULL));
        //On recupère la varibale passéee en paramètre du thread
        BUFFER * buff = ((BUFFER *) arg);
        //Je déréference (accède à la valeur) et lui donne une valeur entre 0 et 10
        *buff->ptEcriture=rand()%10;
        //((buff->ptEcriture+1)-&buff->mem[0]) -> permet de savoir de combien de case on a avancé par rapport a buff[0]
        //Ensuite on fait un modulo pour eviter les déplacement de tableau et donc pour boucler l'ecriture
        //On obtient un nombre entre 0 et BUFFER_SIZE, c'est en fait l'indice du buffer à acceder pour la prochaine fois
        //Du coup on fait adresse du premier element + indice pour obtenir l'adresse en mémoire de l'élément
        buff->ptEcriture = &buff->mem[0]+(((buff->ptEcriture+1)-&buff->mem[0])%SIZE_BUFFER);
        //On attend un nombre random de secondes pour eventuellement simuler un ecrivain plus rapide que le lecteur
        pthread_mutex_unlock(&mutexBufferEcriture);
        usleep((rand()%10)*100000);

    }

}
void * Conso(void * arg){
    puts("Je suis un consommateur");
    while (true) {
        pthread_mutex_lock(&mutexBufferLecture);
        //A l'inverse de Prod, je pose un jeton pour indiquer l'avancement
        V(sem_create(SEM_BUFFER, NULL));
        BUFFER * buff = ((BUFFER *) arg);
        //%-3d permet de dire qu'on écrit sur 3 caractère et si le chiffre est trop court on laisse du blanc, juste pour faire beau
        printf("%-3d",*buff->ptLecture);
        //Il faut flush sinon rien ne s'affiche a la console avant la fin de l'exec
        fflush(stdout);
        buff->ptLecture = &buff->mem[0]+(((buff->ptLecture+1)-&buff->mem[0])%SIZE_BUFFER);

        pthread_mutex_unlock(&mutexBufferLecture);
        usleep((rand()%10)*100000);
    }

}

int main(int argc, char** argv) {

    if(argc !=3)
        perror("Il manque des arguments");

    srand(time(NULL));
    sem_delete(sem_create(SEM_BUFFER, NULL));
    sem_create(SEM_BUFFER, SIZE_BUFFER);


    //Initialisation du Buffer pour que les pointeurs soient sur la première case
    BUFFER  buff = {
            .ptEcriture=&buff.mem[0],
            .ptLecture=&buff.mem[0]

    };

    for (int i = 0; i < atoi(argv[2]); ++i) {
        pthread_t pthread;
        pthread_create(&pthread, NULL, Prod, (void *)&buff);
        usleep(500000);
    }
    for (int i = 0; i < atoi(argv[1]); ++i) {
        pthread_t pthread;
        pthread_create(&pthread, NULL, Conso, (void *)&buff);
        usleep(500000);
    }

    pause();


}