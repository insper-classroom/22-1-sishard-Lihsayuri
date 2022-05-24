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

int matar_tudo = 0;
struct sigaction s;

int conta_num_char(char string[]){
    int count = 0;  
      
    //Counts each character except space  
    for(int i = 0; i < strlen(string); i++) {  
        if(string[i] != '\n')  
            count++;  
    }  

    return count+1;
}

void sig_handler(int num) {
    printf("Chamou Ctrl+C\n");

    char answer[1];

    printf("Você quer mesmo encerrar tudo?");
    scanf("%s", answer);

    if (strcmp(answer, "y") == 0 || strcmp(answer,"Y") == 0){
        kill(0, SIGINT);

        s.sa_handler = SIG_DFL;
        sigaction(SIGINT, &s, NULL);
        raise(SIGINT);
    } 
}


int main(int argc, char *argv[])
{

    s.sa_handler = sig_handler; // aqui vai a função a ser executada
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, NULL);
    char *log;
    char str_flag;
    char *doc;



    if (argv[1][1] == 'l'){
        log = argv[2];
        str_flag = argv[3][1];
        doc = argv[4];
    } else {
        str_flag = argv[1][1];
        doc = argv[2];
        if (argc > 3){
            log = argv[4];
        }
    }

    printf("%c\n", str_flag);
    printf("%s\n", doc);


    char buf[100];
    char string_programas[10000];
    // printf("%s \n", str_flag);
    int fd2 = open(log, O_WRONLY | O_CREAT, 0755);

    if (str_flag == 'f')
    {
        int fd1 = open(doc, O_RDONLY);
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
                char *log1;
                log1 = malloc(sizeof(char)*10000);
                sprintf(log1, "starting %s (pid = %d) \n", programs[i], filho);
                write(fd2, log1, conta_num_char(log1));


                // printf("PIDPAI %d\n", getpid());

                lista[i].pid = filho;
                lista[i].program = programs[i];
            }

        }


        while(1){

            int status = 0;
            pid_t childpid = wait(&status);

            for (int j = 0; j < programs_counter; j++){
                if (lista[j].pid == childpid){
                    char *log2;
                    log2 = malloc(sizeof(char)*10000);
                    sprintf(log2, "program %s (%d) finished (EXITED=%d, EXITSTATUS=%d, SIGNALED=%d, SIGNAL=%d, SIGNALSTR=%s) \n", lista[j].program, 
                    (int)childpid, (int)WIFEXITED(status), (int)WEXITSTATUS(status), (int)WIFSIGNALED(status), WTERMSIG(status), strsignal(WTERMSIG(status)));
                    write(fd2, log2, conta_num_char(log2));


                    pid_t filho = fork();
                    if (filho == 0){
                        char *log3;
                        log3 = malloc(sizeof(char)*10000);
                        sprintf(log3, "restarting %s (oldpid=%d, newpid=%d)\n", lista[j].program, childpid, getpid());
                        write(fd2, log3, conta_num_char(log3));
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


    if (str_flag == 'p'){

        pid_t filho = fork();
        if (filho == 0)
        {
            char *log1;
            log1 = malloc(sizeof(char)*10000);
            sprintf(log1, "starting %s (pid = %d) \n", doc, getpid());
            write(fd2, log1, conta_num_char(log1));
            char *args[] = {doc, NULL};
            execvp(doc, args);
        }

        while(1){

            int status;
            pid_t childpid = wait(&status);

            printf("Parent knows child %d is finished. \n", (int)childpid);
            char *log2;
            log2 = malloc(sizeof(char)*10000);
            sprintf(log2, "program %s (%d) finished (EXITED=%d, EXITSTATUS=%d, SIGNALED=%d, SIGNAL=%d, SIGNALSTR=%s) \n", doc, 
            (int)childpid, (int)WIFEXITED(status), (int)WEXITSTATUS(status), (int)WIFSIGNALED(status), WTERMSIG(status), strsignal(WTERMSIG(status)));
            write(fd2, log2, conta_num_char(log2));

            if (childpid > 0){
                pid_t filho = fork();
                if (filho == 0){
                    char *log3;
                    log3 = malloc(sizeof(char)*10000);
                    sprintf(log3, "restarting %s (oldpid=%d, newpid=%d)\n", doc, childpid, getpid());
                    write(fd2, log3, conta_num_char(log3));
                    char *args[] = {doc, NULL};
                    execvp(doc, args);
                } 
            }
        }
    }
        
    return 0;
}