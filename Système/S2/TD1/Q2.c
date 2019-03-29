#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <string.h>

void eliminer_zombie(int sig) {
    int exit_cond;
    pid_t pid;

    printf("Attente de la terminaison du fils...\n");
    pid = wait(&exit_cond);

    if (WIFEXITED(exit_cond)) {
        printf("%d : termine correctement : %d\n", pid, WEXITSTATUS(exit_cond));
    } else {
        printf("%d : mal termine : %d\n", pid, WTERMSIG(exit_cond));
    }
}

int main(int argc, char *argv[]) {
    int status;
    char cmd[] = "ls";
    char options[] = "-l";

    int pfd[2];
    if (pipe(pfd) == -1) {
        perror("impossible de creer le tube");
    }

    signal(SIGCHLD, eliminer_zombie);
    
    switch (fork()) {
        case 0: {
            close(STDOUT_FILENO);
            close(pfd[0]);
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[1]);

            execlp(cmd, cmd, options, NULL, NULL);

            exit(2);
        }
        break;

        default: {
            char buf[512];

            close(pfd[1]);
            read(pfd[0], buf, 512);

            printf("%s\n", buf);

            sleep(1);
            exit(EXIT_SUCCESS);
            break;
        }
    }

    return EXIT_SUCCESS;
}