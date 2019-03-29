#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define CAPACITE_ENTREPOT 10
#define INTERVALLE_INVENTAIRE 1

int capacite_max_entrepot (){
    return CAPACITE_ENTREPOT;
};

void *activite_entrepot(void *pdata) {

    int * stock = (int *)(pdata);

    while(1) {
        printf("Entrepot: Stock actuel: %d\n", *stock);
        sleep(INTERVALLE_INVENTAIRE);
    }
}

int creation_entrepot(pthread_t *t, int * stock) {
    return pthread_create(t, NULL, activite_entrepot, stock);
}