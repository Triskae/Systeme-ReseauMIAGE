#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TEMPS_PAR_PRODUCTION 1

int capacite_max_entrepot();

void *activite_usine(void *pdata) {
    while (1) {
        int *pointeurStock = (int *)(pdata);
        if (*pointeurStock < capacite_max_entrepot()) {
            (*pointeurStock) ++;
            printf("Usine: Production d'une voiture\n");
        } else {
            printf("Usine: Arret de la production\n");
        }
        sleep(TEMPS_PAR_PRODUCTION);
    }
}

int creation_usine(pthread_t *t, int * stock) {
    return pthread_create(t, NULL, activite_usine, stock);
}