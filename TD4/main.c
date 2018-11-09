#include <stdio.h>
#include <stdlib.h>
#include "image.h"

void usage(){
    fprintf(stderr,"image finitial.pgm ftransforme.pgm\n");
}

int main(int argc, char *argv[]){
    FILE *f;
    image *I;

    if (argc < 3){
        usage();
        return -1;
    }

    /* Lecture de l'image */
    f = fopen(argv[1],"r");
    if (f == NULL)
        fprintf(stderr,"Erreur d'ouverture du fichier initial \n");
    I = lecture_image(f);
    fclose(f);

    /* Traitement de l'image */
    inverse_image(I);

    /* Ecriture de l'image */
    f = fopen(argv[2],"w");
    ecriture_image(f,I);
    fclose(f);

    return EXIT_SUCCESS;
}