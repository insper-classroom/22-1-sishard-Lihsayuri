#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
// Edite este arquivo para responder o lab!
// Adicione funções e modifique o que for preciso!

typedef struct pidPrograma
{
    int pid;
    char *program;
    int status;
} pidProgram;

int main(int argc, char *argv[])
{

    int wstatus = 0;
    char buf[100];

    char *str_flag = argv[1];

    char string_programas[10000];
    // printf("%s \n", str_flag);

    if (str_flag[1] == 'f')
    {
        int fd1 = open(argv[2], O_RDONLY);
        int tamanho = 1;

        while (tamanho > 0)
        {
            tamanho = read(fd1, buf, 99);
            if (tamanho > 0)
            {
                buf[tamanho] = '\0';
                strcat(string_programas, buf);
            }
        }

        close(fd1);

        char **programs = NULL;
        int programs_counter = 0;

        const char s[2] = "\n";
        char *token;

        /* get the first token */
        token = strtok(string_programas, s);

        /* walk through other tokens */
        while (token != NULL)
        {
            programs_counter++;

            programs = realloc(programs, programs_counter * sizeof(char *));
            programs[programs_counter - 1] = malloc(sizeof(char) * 1000);

            strcpy(programs[programs_counter - 1], token);
            // printf("%s \n" , programs[programs_counter-1]);

            token = strtok(NULL, s);
        }

        printf("%d \n", programs_counter);

        pidProgram *lista = malloc(sizeof(pidProgram) * programs_counter);

        for (int i = 0; i < programs_counter; i++)
        {
            pid_t filho = fork();
            if (filho == 0)
            {
                char *args[] = {programs[i], NULL};
                execvp(programs[i], args);
            }
            else{
                printf("Filho %s pid: %d \n", programs[i], filho);
                lista[i].pid = filho;
                lista[i].program = programs[i];
            }

        }


        // wait(&wstatus);

        // for(int i = 0; i < programs_counter; i++) {
        while(1){
            int status = 0;
            pid_t childpid = wait(&status);
            printf("Parent knows child %d is finished. \n", (int)childpid);
            for (int j = 0; j < programs_counter; j++){
                if (lista[j].pid == childpid){
                    pid_t filho = fork();
                    if (filho == 0){
                        char *args[] = {lista[j].program, NULL};
                        execvp(lista[j].program, args);
                    } else {
                        lista[j].pid = filho;
                        lista[j].program = lista[j].program;
                    }
                }
            }
        }

    }

    if (str_flag[1] == 'p'){
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
    }

    return 0;
}