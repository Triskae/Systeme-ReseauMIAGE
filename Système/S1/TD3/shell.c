# include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include <zconf.h>

# define LSH_TOK_BUFSIZE 64
# define LSH_TOK_DELIM " \t\n"

static int env_state = 1;

void intHandler(int sig) {
    printf("Le CTRL + C n'est pas autorisé");
    fflush(stdout);
    signal(SIGINT, intHandler);
}

void freeArgs(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        free(args[i]);
        i++;
    }
    free(args);
}

int checkEnv() {
    if (getenv("FORBIDDEN") == NULL) return 1;
    return 0;
}

char *sh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

//Modification pour que cette fonction soit plus générique
//Elle peut maintenant traiter des chaines avec un séparateur donné en param
char **sh_split_line(char *line, char *TOKEN) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    char *linecopy = strdup(line);
    if (!tokens) {
        fprintf(stderr, " lsh : allocation error \n");
        exit(EXIT_FAILURE);
    }
    token = strtok(linecopy, TOKEN);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, " lsh : allocation error \n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN);
    }
    tokens[position] = NULL;
    return tokens;
}

int forbidden_commands(char **command_line) {
    int i = 0;
    int j = 0;
    char *path = getenv("FORBIDDEN");

    //Exclure la suppression d'un mot interdit de la vérification
    if (strcmp(command_line[0], "rmf") == 0) return 0;

    //Copie du contenu de getenv, car faut pas toucher au pointeur retourné par getenv !!!
    char *pathCopy = strdup(path);
    char **pathTab = sh_split_line(pathCopy, ":");
    while (command_line[i] != NULL) {
        while (pathTab[j] != NULL) {
            if (strcmp(command_line[i], pathTab[j]) == 0) {
                return 1;
            }
            j++;
        }
        j = 0;
        i++;
    }
    return 0;
}


void sh_free(void *line) {
    free(line);
}

int addEnv(char *envToAdd) {
    char *path = getenv("FORBIDDEN");
    //Copie du contenu de getenv, car faut pas toucher au pointeur retourné par getenv !!!
    char *pathCopy = strdup(path);

    //10 pour avoir un peu de marge
    char *tempString = malloc(strlen(path) + strlen(envToAdd) + 10);

    strcat(tempString, pathCopy);
    strcat(tempString, ":");
    strcat(tempString, envToAdd);

    int ret = setenv("FORBIDDEN", tempString, 1);
    printf("La variable d'environnement %s a bien été ajoutée\n", envToAdd);

    free(pathCopy);
    free(tempString);
    return 0;
}

int removeEnv(char *envToRemove) {
    char *path = getenv("FORBIDDEN");
    //Copie du contenu de getenv, car faut pas toucher au pointeur retourné par getenv !!!
    char *pathCopy = strdup(path);

    //On enleve la marge ajouté avec le newf
    char *tempString = malloc(strlen(path) - strlen(envToRemove));
    char **pathTab = sh_split_line(pathCopy, ":");

    int i = 0;
    int find = 0;

    while (pathTab[i] != NULL) {
        if (strcmp(pathTab[i], envToRemove) != 0) {
            strcat(tempString, pathTab[i]);
            strcat(tempString, ":");
        } else {
            find = 1;
        }
        i++;
    }
    //On retire les deux points useless de la fin
    tempString[strlen(tempString)-1] = '\0';

    int ret = setenv("FORBIDDEN", tempString, 1);

    if (find == 1) {
        printf("La variable d'environnement %s a bien été supprimée\n", envToRemove);
    } else {
        printf("La variable d'environnement %s n'existe pas dans FORBIDDEN\n", envToRemove);
    }
    free(pathCopy);
    free(tempString);
    return 0;
}

int internal_commands(void **args) {
    if (strcmp(args[0], "exit") == 0) return 1;
    else if (strcmp(args[0], "newf") == 0) {
        if (args[1] != NULL && args[2] == NULL) {
            addEnv(args[1]);
        } else {
            printf("Usage: newf <element>\n");
        }
    } else if (strcmp(args[0], "rmf") == 0) {
        if (args[1] != NULL && args[2] == NULL) {
            removeEnv(args[1]);
        } else {
            printf("Usage: rmf <element>\n");
        }
    }

    return 0;
}

int sh_execute(void **args) {
    //Eviter le crash lorsque enter est appuyé sans rien
    if (args[0] == NULL) return 1;
    int i = 0;
    if (internal_commands(args) == 1) return 0;
    int exit_cond;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        exit(0);
    } else {
        pid = wait(&exit_cond);
    }
    return 1;
}


void sh_loop() {
    signal(SIGINT, intHandler);
    char *prompt = " > ";
    char *line;
    char **args;
    int status;

    do {
        printf(" %s ", prompt);
        fflush(stdout);
        line = sh_read_line(); /* Lecture de la ligne de commande */
        args = sh_split_line(line, LSH_TOK_DELIM);  /* Dont on en extrait les args */

        if (forbidden_commands(args) == 0) {
            status = sh_execute(args);
        } else {
            printf("Travail au lieu de jouer\n");
        }
        //freeArgs(args);
        free(line);
    } while (status);
}


int main(int argc, char *argv[]) {
    // Init : Load config files, if any
    // Interp : Run Command loop
    //Catch le CTRL + C
    if (checkEnv() == 0) {
        sh_loop();
    } else {
        perror("FORBIDDEN N'EXISTE PAS\n");
        return EXIT_FAILURE;
    }
    // Termin : Perform any shutdown / cleanup
    return EXIT_SUCCESS;
}