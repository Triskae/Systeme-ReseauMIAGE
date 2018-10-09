#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
    int pid;
    switch(pid = fork()){
        case -1:
            perror("Erreur de fork");
            exit(1);
        case 0:
            printf("Je suis le fils : PID %d et le PId de mon pere est %d\n", getpid(), getppid());
            printf("Je suis le fils et je meurs : PID %d\n", getpid());
        break;
        default:
        printf("\nJe suis le pere : PID %d\n", getpid());
        printf("Qu'est devenu mon fils %d ?\n", pid);
        printf("Vous avez 30sec pour lancer un ""ps -e -f"" et constater qu'il est zombi !\n");
        sleep(30);
        printf("Je suis le pere, et je meurs : PId %d\n", getpid());
}
printf("\n");
exit(0);
}
