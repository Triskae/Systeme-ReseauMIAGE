#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NB_CONCESSION 3
#include "structure.h"

int creation_usine(pthread_t *t, int *stock);
int creation_concession(pthread_t *t, struct concessionaire_data * data);
int creation_entrepot(pthread_t *t, int *stock);
pthread_mutex_t lock;

int main(int argc, char*argv[]){
    int r = 1;
    int i;

    //Stock utilisé par tous les établissements



    pthread_mutex_lock(&lock);
    int stock = 0;
    pthread_mutex_unlock(&lock);

    //Usine
    pthread_t usine;

    //Entrepot
    pthread_t entrepot;

    //Concession
    pthread_t concession[NB_CONCESSION];
    int stockConcession = 0;



    r = creation_usine(&usine, &stock);


    if (r != 0)
        fprintf(stderr,"Usine echec !");

    r=creation_entrepot(&entrepot, &stock);

    if (r != 0)
        fprintf(stderr,"Entrepot echec !");



    for(i=0;i<NB_CONCESSION;i++){


        struct concessionaire_data * data = malloc(sizeof(struct concessionaire_data));

        (*data).pid = i;
        (*data).p_stock_usine = &stock;


        r=creation_concession(&(concession[i]),data);

        if (r != 0)
            fprintf(stderr,"Concession echec !");
    }

    pthread_join(usine, NULL);
    pthread_join(entrepot, NULL);

    for(i=0;i<NB_CONCESSION;i++) {
        pthread_join(concession[i], NULL);
    }
    return 0;
}