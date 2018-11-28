/*
 * extract MP3 tag frames
 * *
 * Author: Gilles MENEZ
 * Created: 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <arpa/inet.h>

#include "util.h"
#include "id3_tagheader.h"
#include "id3_frame.h"


/*=====================================================*/
/* Pour comprendre comment parcourir un fichier mp3.

   On rend la position en fin de lecture : normalement 
   on devrait etre en fin de tag.
*/

off_t mp3_read(int fd) {
    int nb_lu;
    int i;

    /* Tag Header analysis ---------------------*/
    tag_header th; /* on va recuperer le header du tag
		    car on en aura besoin pour parcourir les frames */
    nb_lu = id3_read_tagheader(fd, &th);
    if (nb_lu != 10) return nb_lu;

    /* Tag frames analysis ---------------------*/
    do {
        nb_lu = id3_read_frame(fd, th.tailletag);
    } while (nb_lu > 0);

    /* Pour verifier ou on est a la fin de la lecture du tag ? -*/
    off_t fintag = lseek(fd, 0, SEEK_CUR);

#if DEBUG
    printf("\nFIN TAG : %d\n", fintag);
#endif

    return fintag;
}

char **ls_mp3files_inarray(char *dirname) {
    int i, nbFichiers;
    DIR *dir;
    struct dirent *entree;
    char **mp3files;

    dir = opendir(dirname);
    if (dir == NULL) {
        fprintf(stderr, "Ouverture du repertoire %s impossible.\n", dirname);
    }

    nbFichiers = 0;
    while ((entree = readdir(dir)) != NULL) {
        if (entree->d_type == DT_REG) {
            char *extension;

            if ((extension = strrchr(entree->d_name, '.')) != NULL) {
                if (strcmp(extension, ".mp3") == 0) {
                    nbFichiers++;
                }
            }
        }
    }
    closedir(dir);

    mp3files = malloc(nbFichiers * sizeof(char *));

    dir = opendir(dirname);
    if (dir == NULL) {
        fprintf(stderr, "Ouverture du repertoire %s impossible.\n", dirname);
    }

    i = 0;
    while ((entree = readdir(dir)) != NULL) {
        if (entree->d_type == DT_REG) {
            char *extension;

            if ((extension = strrchr(entree->d_name, '.')) != NULL) {
                if (strcmp(extension, ".mp3") == 0) {
                    mp3files[i] = entree->d_name;
                    i++;
                }
            }
        }
    }
    mp3files[i] = NULL;
    closedir(dir);

    return mp3files;
}

/*=====================================================*/

int main(int argc, char *argv[]) {
    FILE *f;
    char *filename;
    int nb_lu;
    int i;

    /* Verification de l'appel du programme ----*/
    if (argc != 2) { /* usage -- print usage message and exit */
        fprintf(stderr, "Usage: %s mp3file\n", argv[0]);
        exit(1);
    } else
        filename = argv[1];

    /* Ouverture du fichier --------------------*/
    if (!(f = fopen(filename, "r"))) {
        perror(filename);
        exit(1);
    }

    int fd = fileno(f); /* Get descriptor from FILE * */

    /* 1) Parcours d'un fichier mp3 */
    mp3_read(fd);

    printf("\n");
    sync();


    lseek(fd, 0, SEEK_SET);
    char *id = "TIT2";
    char contenu[10000] = " ";
    int cr = mp3_get_frame_from_id(fd, id, contenu);
    if ((cr != -1) && (contenu[0] != '\0')) {
        contenu[30] = '\0';
        printf("% s \n", contenu);
    } else {
        printf("%s \n ", "pas trouvé");
    }

    return 0;
}

/*=====================================================*/
