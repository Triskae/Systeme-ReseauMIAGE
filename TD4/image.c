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


    if (fgets(line, 60, f) != NULL){
        strcat(I->nm, line);
    } else {
        printf("Erreur null");
    }

    fgets(line, 60, f);

    do {
        strcat(I->comments, line);
        strcpy(line, "");
    }while(fgets(line, 60, f)[0] == '#');



    printf("%s\n", I->nm);
    printf("%s\n", I->comments);


    char * token = strtok(line, " ");
    I->nl = atoi(token);


    while( token != NULL ) {
        token = strtok(NULL, " ");
        if (token != NULL) I->nc = atoi(token);
    }

    printf("%d\n", I->nl);
    printf("%d\n", I->nc);

    I->ng = atoi(fgets(line, 60, f));

    int i = 0;
    //fscanf
    //Meme tete de lecture
    while(fgets(line, 60, f) != NULL) {

    }
    return I;
}


void ecriture_image(FILE *f, image *img) {
    fwrite(img->nm, 1, sizeof(img->nm), f);

    for (int i = 0; i < (sizeof(img->comments)/ sizeof(char))/5000; ++i) {
        fwrite(img->comments, 1, sizeof(img->comments), f);
    }

    fprintf(f, "%d %d\n", img->nl, img->nc);
    fprintf(f, "%d\n", img->ng);

    for (int i = 0; i < img->nl*img->nc; ++i) {
        fprintf(f, "%c\n", img->pixels[i]);
    }

}

void inverse_image(image *img) { // Im[i] = MaxLevel - Im[i]

    for (int i = 0; i < img->nl * img->nc; ++i) {
        img->pixels[i] = img->ng - atoi(&img->pixels[i]);

    }
}
