#include <stdio.h>
#include "Djikstra.h"
#include <pthread.h>

#include <ctype.h>

#define SEM_FINISH        6
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/**
 * On crée une structure pour contenir notre mutex, argv et
 * notre variable à protéger pour plus de clareté
 */
typedef struct {
    int indice;
    pthread_mutex_t mutex;
    char const **argv;
} mutex;

mutex  indice=  {
        //On commence au troisième argument du tableau argv
        // car premier est le chemin du fichier binaire et le
        // deuxième est le nombre de thread
    .indice = 2,
    .mutex=PTHREAD_MUTEX_INITIALIZER
};


int argc;
pthread_t *threads;


void * thread(void * arg){
    int x;
    do {
        {
            //On s'apprête à modifier une variable protegée
            pthread_mutex_lock(&indice.mutex);
            x = indice.indice++;
            pthread_mutex_unlock(&indice.mutex);
        }
        //Si on a calculé toutes les lignes on sort de la boucle infinie
        if(x>=arg-1) break;

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
    //Une fois tous les fichiers traités on débloque le thread principal pour qu'il termine le programme
        V(sem_create(SEM_FINISH, NULL));
}
/**
 *
 * @param _argc
 * @param _argv Premier argument : nombre de threads à créer
 *              Arguments suivants : chemins des fichiers à transformer
 * @return
 */
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
       pthread_create(&threads[y], NULL, thread,argc);
    }

    P(sem_create(SEM_FINISH,NULL));
    return 0;
}

