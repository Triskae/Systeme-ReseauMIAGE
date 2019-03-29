#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    int pid, ppid;
    switch(pid = fork())
    {
        case -1:
            perror("Erreur de fork");
            exit(1);
        case 0:
            ppid = getppid();
            printf("Je suis le fils (%d) et le PID de mon pere est (%d)_n", getpid(), ppid);

            printf("Vous avez 15 sec pour lancer un ""ps -e -f"" !\n");

            printf("\nJe suis le fils (%d) et le PID de mon pere est (%d)\n", getpid(), getppid());

            sleep(40);
            if (getppid() != ppid)
                printf("Je suis donc devenu orphelin ! \n");
            break;
        default:
            printf("Je suis le pere (%d) \n", getpid());
            sleep(15);
            printf("Je suis  le pere (%d) et je meurs AVANT mon fils (%d)\n", getpid(), pid);
    }

    printf("FIN \n");
}
