#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// Edite este arquivo para responder o lab!
// Adicione funções e modifique o que for preciso!



int main(int argc, char *argv[]) {

    int wstatus = 0;
    int status = 1;


    printf(" Isso aqui: %s \n", argv[2]);
    while (status != 0){
        pid_t filho = fork();
        if (filho == 0){
            printf("Meu pid: %d\n", getpid());
            char *args[] = {argv[2], NULL};
            execvp(argv[2], args);
        }
        else {
            wait(&wstatus);        
            printf("Status aqui %d \n", WEXITSTATUS(wstatus));
            status = 1;
        }

    }


   
    return 0;
}