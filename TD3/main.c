# include <stdio.h>
# include <stdlib.h>
#include <string.h>

# define LSH_TOK_BUFSIZE 64
# define LSH_TOK_DELIM " \t\n"


char *sh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **sh_split_line(char *line, char * TOKEN) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    if (!tokens) {
        fprintf(stderr, " lsh : allocation error \n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, TOKEN);
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

int forbidden_commands(char ** command_line) {
    //Utiliser strstr
    // Ancienne version, ajouter les mots interdits dans la variable d'environnement FORBIDDEN
    //char* forbidden_coms [] = {"mp3", "mp4", "youtube", "avi"};


    //for (int i = 0; i < sizeof(forbidden_coms)/ sizeof(char*); ++i) {
    //    if (strstr(line, forbidden_coms[i]) != NULL) return 1;
    //}

    char ** args = sh_split_line(getenv("FORBIDDEN"),":");
    int i = 0;
    int j = 0;
    char * var = "";
    char * var2 = "";

    while (command_line[i] != NULL) {
        while (args[j] != NULL) {
            var = command_line[i];
            var2 = args[j];
            if (strcmp(command_line[i], args[j]) == 0) return 1;
            j++;
        }
        i++;
    }
    return 0;
}


void sh_free(void *line) {
    free(line);
}

int internal_commands(void **args) {
    if (strcmp(args[0], "exit") == 0) return 1;
    else return 0;
}

int sh_execute(void **args) {
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


void sh_loop(void) {
    char * prompt = " > ";
    char *line;
    char **args;
    int status;

    do {
        printf(" %s ", prompt);
        fflush(stdout);
        line = sh_read_line(); /* Lecture de la ligne de commande */

        char ** args_copy = malloc(sizeof(char **));
        args = sh_split_line(line, LSH_TOK_DELIM);  /* Dont on en extrait les args */

        strcpy(args_copy, args);


        if (forbidden_commands(args_copy) == 0) {
            status = sh_execute(args);
        } else {
            printf("Travail au lieu de jouer\n");
        }

        //sh_free(line);
        //sh_free(args);
    } while (status);
}


int main(int argc, char *argv[]) {
    // Init : Load config files, if any
    // Interp : Run Command loop
    sh_loop();
    // Termin : Perform any shutdown / cleanup
    return EXIT_SUCCESS;
}