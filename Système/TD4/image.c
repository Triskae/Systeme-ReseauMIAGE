//
// Created by Filipe Doutel Silva on 25/10/2018.
//


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

image *lecture_image(FILE *f) {
    char line[100];
    image *I = malloc(sizeof(image));

    //Traitement du numéro magique
    if (fgets(line, 60, f) != NULL){
        strcat(I->nm, line);
    } else {
        printf("Erreur null");
    }

    fgets(line, 60, f);

    //Traitement des commentaires
    do {
        strcat(I->comments, line);
        strcpy(line, "");
    }while(fgets(line, 60, f)[0] == '#');



    printf("%s\n", I->nm);
    printf("%s\n", I->comments);


    //Traitement du numéro de ligne
    char * token = strtok(line, " ");
    I->nl = atoi(token);


    //Traitement du numéro de numéro de colonnes
    while( token != NULL ) {
        token = strtok(NULL, " ");
        if (token != NULL) I->nc = atoi(token);
    }

    printf("%d\n", I->nl);
    printf("%d\n\n", I->nc);


    I->ng = atoi(fgets(line, 60, f));

    int i = 0;
    //fscanf
    //Meme tete de lecture
    int pixel;

    for (int j = 0; j < I->nl * I->nc; ++j) {
        fscanf(f, "%d", &pixel);
        I->pixels[j] = pixel;

    }

    for (int j = 0; j < I->nl * I->nc; ++j) {
        printf("%c", I->pixels[j]);
    }
    return I;
}


void ecriture_image(FILE *f, image *img) {
    fprintf(f, "%s\n", img->nm);
    fprintf(f, "%s", img->comments);
    fprintf(f, "%d %d\n", img->nl, img->nc);
    fprintf(f, "%d\n", img->ng);

    for (int i = 0; i < img->nc * img->nl; ++i) {
        fprintf(f, "%d\n", img->pixels[i]);
    }

}

void inverse_image(image *img) { // Im[i] = MaxLevel - Im[i]

    for (int i = 0; i < img->nl * img->nc; ++i) {
        img->pixels[i] = img->ng - img->pixels[i];

    }
}
