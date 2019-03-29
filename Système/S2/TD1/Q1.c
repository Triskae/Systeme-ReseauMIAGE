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
    int shmid;
    char *virtualaddr;
    sem_t *put;
    sem_t *get;
    key_t key;
    pid_t pid;

    key = ftok(argv[0], 'R');
    shmid = shmget(key, 1024, 0644|IPC_CREAT);

    if (0 > shmid) {
        perror("Shared Mem creation error\n");
        exit(EXIT_FAILURE);
    }

    virtualaddr = shmat(shmid, (void *)0, 0);

    put = sem_open("/put", O_CREAT|O_RDWR, 0644, 0);
    get = sem_open("/get", O_CREAT|O_RDWR, 0644, 0);

    switch (fork()) {
        case -1: {
            printf("Error forking child 1!\n");
            exit(1);
        }

        case 0: {
            char buf[512];

            put = sem_open("/put", O_RDWR);
            get = sem_open("/get", O_RDWR);

            while (1) {
                sem_wait(get);

                strcpy(buf, virtualaddr);
                printf("Message received by child 2 : %s", buf);

                make_message(2, buf);
                sleep(1);
                strcpy(virtualaddr, buf);

                sem_post(put);
            }

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

            put = sem_open("/put", O_RDWR);
            get = sem_open("/get", O_RDWR);

            sem_post(get);

            while (1) {
                sem_wait(put);

                strcpy(buf, virtualaddr);
                printf("Message received by child 1 : %s", buf);

                make_message(1, buf);
                sleep(1);
                strcpy(virtualaddr, buf);

                sem_post(get);
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

    sem_unlink("/put");
    sem_unlink("/get");

    shmctl(shmid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}