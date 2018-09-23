#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "structure.h"

#define CAPACITE_CONCESSION 10
#define TEMPS_MOYEN_ENTRE_VENTE 5

void *activite_concession(struct concessionaire_data *pdata) {
    int stock_concession = 0;

    struct concessionaire_data *data = pdata;

    while(1) {
        printf("Concession %d: Stock de %d voiture(s)\n", (*data).pid, stock_concession);

        //Dans le cas ou on peu encore remplir la capacité de la concession et qu'il en reste dans les dépots
        while (stock_concession < CAPACITE_CONCESSION && *(*data).p_stock_usine > 0) {
            stock_concession = stock_concession + 1;
            *(*data).p_stock_usine = *(*data).p_stock_usine-1;
        }

        if (stock_concession > 0) {
            printf("Concession %d: Vente d'une voiture\n", (*data).pid);
            stock_concession = stock_concession - 1;
        }
        sleep(TEMPS_MOYEN_ENTRE_VENTE);
    }

}

int creation_concession(pthread_t *t, int *data) {
    return pthread_create(t, NULL, activite_concession, data);
}