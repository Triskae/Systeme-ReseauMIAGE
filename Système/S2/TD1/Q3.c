#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <string.h>

void make_message(int num, char *message) {
    char buftime[26];
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buftime, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    sprintf(message, "%s %d at %s\n", "Hello, I'm child number", num, buftime);
}

int main(int argc, char *argv[]) {
    int i;
    pid_t pid;

    int p1p2[2];
    if (pipe(p1p2) == -1) {
        fprintf(stderr, "Pipe failed");
        return EXIT_FAILURE;
    }

    int p2p1[2];
    if (pipe(p2p1) == -1) {
        fprintf(stderr, "Pipe failed");
        return EXIT_FAILURE;
    }
    
    switch (fork()) {
        case -1: {
            printf("Error forking child 1!\n");
            exit(1);
        }

        case 0: {
            char buf[512];

            while (1) {
                read(p1p2[0], buf, 512);
                printf("Message received by child 2 : %s", buf);

                memset(buf, 0, 512);

                make_message(2, buf);
                sleep(1);
                write(p2p1[1], buf, strlen(buf)+1);
                fflush(stdout);
            }

            _exit(EXIT_SUCCESS);
        }
        break;

        default: {
            break;
        }
    }

    switch (fork()) {
        case -1: {
            printf("Error forking child 2!\n");
            exit(1);
        }

        case 0: {
            char buf[512];

            make_message(1, buf);
            write(p1p2[1], buf, strlen(buf)+1);
            fflush(stdout);

            while (1) {
                read(p2p1[0], buf, 512);
                printf("Message received by child 1 : %s", buf);

                make_message(1, buf);
                sleep(1);
                write(p1p2[1], buf, strlen(buf)+1);
                fflush(stdout);
            }

            _exit(EXIT_SUCCESS);
        }
        break;

        default: {
            break;
        }
    }

    printf("After forking, parent waiting for children completion/terminate...\n\n");
    for (i = 0; i < 2; i++) {
        if (wait(NULL) == -1) {
            printf("Error waiting\n");
            exit(EXIT_FAILURE);
        }
    }

    close(p1p2[0]);
    close(p1p2[1]);

    close(p2p1[0]);
    close(p2p1[1]);

    exit(EXIT_SUCCESS);
}