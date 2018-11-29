# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

int main(int argc, char *argv[]) {
    int exit_cond;

    pid_t tabProc [5];

    for (int i = 0; i < 5; ++i) {
        if ((tabProc[i] = fork()) == -1) {
            printf("Erreur lors du fork");
        } else if (tabProc[i] == 0) {
            printf(" Pid du fils = %d\n", getpid());
            sleep(10);
            exit(0);
        }
    }


    for (int i = 0; i < 5; ++i) {
        tabProc[i] = wait(&exit_cond);
        if (WIFEXITED(exit_cond))
            printf(" Le fils %d s ’ est termine correctement : %d\n ",
                   tabProc[i], WEXITSTATUS(exit_cond));
        else
            printf(" Le fils %d s ’ est mal termine : %d\n", tabProc[i], WTERMSIG(exit_cond));
    }
    
    sleep(30);
}

