//
// Created by Filipe Doutel Silva on 25/10/2018.
//

#include <stdio.h>

#ifndef IMAGEH
#define IMAGEH

#define NMAX 3000
#define SIZECOMMENT 5000

typedef struct {
    char nm[5];  /* Nombre magique */
    char comments[SIZECOMMENT];
    int nl, nc;  /* Nombre ligne/colonnes */
    int ng;      /* Nombre de gris */
    unsigned char pixels[NMAX*NMAX]; /* Pas top et ... */
    /* si vous le sentez => heap !  */
    /* Par contre le 2D est inutile */
} image;

/* Prototypes */
image *lecture_image(FILE *f);
void ecriture_image(FILE *f, image *img);
void inverse_image(image *img);

#endif