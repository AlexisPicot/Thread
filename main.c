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
    int indiceFichierCourant;
    pthread_mutex_t mutex;
    char const **listeDesFichiers;
} mutex;

mutex indice = {
        //On commence au troisième argument du tableau listeDesFichiers
        // car premier est le chemin du fichier binaire et le
        // deuxième est le nombre de thread
    .indiceFichierCourant = 2,
    .mutex=PTHREAD_MUTEX_INITIALIZER
};


pthread_t *threads;


void * thread(void * arg){
    int x;
    do {
        {
            //On s'apprête à modifier une variable protegée
            pthread_mutex_lock(&indice.mutex);
            x = indice.indiceFichierCourant++;
            pthread_mutex_unlock(&indice.mutex);
        }
        //Si on a calculé toutes les lignes on sort de la boucle infinie
        if(x>=arg-1) break;

        FILE* fp1, *fp2;
        int c;
        fp1= fopen (indice.listeDesFichiers[x], "r");
        fp2= fopen (indice.listeDesFichiers[x], "r+");
        if ((fp1== NULL) || (fp2== NULL)) {
            perror ("fopen"); exit (1);
        }
        while (c !=EOF) {
            c=fgetc(fp1);
            if (c!=EOF) fputc(toupper(c),fp2);
        }
        fclose (fp1); fclose (fp2);
        //On dépose un jeton quand on a fini de créer le fichier
        V(sem_create(SEM_FINISH, NULL));

    }while (1);
    //Une fois tous les fichiers traités on s'arrête

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
    indice.listeDesFichiers=_argv;
    int nbThread = MIN(atoi(_argv[1]),_argc);
    pthread_mutex_unlock(&indice.mutex);
    //On supprime le semaphore avant de le recréer afin d'eviter de récupérer un semaphore d'un précedent programme
    sem_delete(sem_create(SEM_FINISH,NULL));
    sem_create(SEM_FINISH,0);

    threads = malloc(sizeof(pthread_t)*nbThread);


    for (int y = 0; y < nbThread; ++y) {
       pthread_create(&threads[y], NULL, thread,_argc);
    }

    //On prend autant de jetons qu'il y a de fichiers à traiter
    for (int y = 2; y < _argc-1; ++y) {
        P(sem_create(SEM_FINISH,NULL));
    }
    return 0;
}

