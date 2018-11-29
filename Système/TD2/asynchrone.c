# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>


void eliminer_zombie (int sig){
/* Handler du signal SIGCHLD */
    int exit_cond ;
    pid_t pid;
    printf (" Attente de la terminaison du fils ...\n");
    pid = wait (& exit_cond );

#if 0
    /* Utilisation alternative d’une gestion non bloquante du wait */
while ( waitpid ( -1 , & sexit_cond , WNOHANG ));
# endif


    if ( WIFEXITED ( exit_cond ))
        printf (" Le fils %d s ’ est termine correctement : %d\n " , pid ,
                WEXITSTATUS ( exit_cond ));
    else
        printf (" Le fils %d s ’ est mal termine : %d\n" , pid ,
                WTERMSIG ( exit_cond ));
}


int main (int argc , char * argv []) {
    struct sigaction action;
    int i;
    pid_t pid;
    for (i=0; i <2 ;i++) {
        pid = fork () ;
        switch (pid) {
            case -1 : perror (" Erreur de création du processus ");
                exit (1) ;
            case 0 : /* on est chez le fils */
                printf (" Pid du fils = %d\n" , getpid () );
                if (i ==0)
                    sleep (5) ; /* Le premier fils meurt au bout de 5 secondes */
                if (i ==1)
                    sleep (10) ; /* Le deuxieme fils meurt au bout de 20 secondes */
                exit (0) ;
        } /* switch */
    } /* for */

/* on est forcement chez le pere */
    printf ("\nPid du pere = %d\n" , getpid () );
/* Attachement du handler du signal SIGCHLD */
    memset(&action , 0, sizeof(action));

    action.sa_handler = eliminer_zombie; /* Fonction handler */

    sigaction ( SIGCHLD , &action , NULL ); /* Attachement de l’action au signal */


/* Le pere peut desormais faire ce qu ’il veut en concurrence de ses
fils */
/* Ce n’est pas tres propre mais je mets ici autant d’actions
qu ’il y a de fils a attendre car chaque fonction sleep est
interrompue par l’execution asynchrone du handler */
    sleep (20) ;
    sleep (20) ;
    printf (" Mort du pere !\n ");
    exit (0) ; /* execute par le pere */
}

