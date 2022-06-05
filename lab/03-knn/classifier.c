#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include "knn.h"
#include "raylib.h"




typedef struct args{
    sem_t *sem_barrier;
    pthread_mutex_t *mutex;
    long *pred_result;
    t_knn_classifier *knn_model;
    double *data;
    long num;
    long *counter;
} barrier;


typedef struct relacao_loading{
    long size_x;
    long *counter;
    char nome[5];
} loading; 


// --------------- THREAD BARREIRA PARA ESPERAR TODAS AS THREADS E LIMITAR SUA EXECUÇÃO SIMULTÂNEA EM 1 NÚMERO -------

void *thread_barreira(void *args) {
    barrier *arg = (barrier *)args;


    sem_wait(arg->sem_barrier);

    pthread_mutex_lock(arg->mutex);

    // printf("Num da thread %ld\n", arg->num);

    pthread_mutex_unlock(arg->mutex);

    *arg->pred_result = knn_predict(arg->knn_model, arg->data);    
  
    *arg->pred_result = knn_predict(arg->knn_model, arg->data);
    
    sem_post(arg->sem_barrier);


    return NULL;
}

//--------- THREAD PARA DESENHAR UMA JANELA QUE MOSTRA UMA BARRA DE PROGRESSO DA EXECUÇÃO DAS THREADS ----------------

void *thread_desenha_run(void *args){
    loading *arg = (loading *)args;

    InitWindow(800, 450, "Executing Window");
    SetTargetFPS(60);

    char texto[100];
    sprintf(texto, "EXECUTING THE THREADS: %s...", arg->nome);

    while (!WindowShouldClose())
    {
        printf("COUNTER EXECUTE: %ld\n", *arg->counter);
        long x = ((float)500/arg->size_x)*(*arg->counter);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(texto, 100, 150, 20, BLACK);
            DrawRectangleLines(100, 200, 500, 60, BLACK);
            DrawRectangle(100, 200, x, 60, BLUE);
            DrawText("LOADING...", 300, 215, 20, GREEN);
        EndDrawing();

        if (*arg->counter == arg->size_x){
            CloseWindow();
        }

    }

    return 0;
}


void *thread_desenha_create(void *args){
    loading *arg = (loading *)args;

    InitWindow(800, 450, "Creating Window");
    SetTargetFPS(60);

    char texto[100];
    sprintf(texto, "CREATING THE THREADS: %s ...", arg->nome);

    while (!WindowShouldClose())
    {
        printf("COUNTER CREATE: %ld\n", *arg->counter);
        long x = ((float)500/arg->size_x)*(*arg->counter);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(texto, 100, 150, 20, BLACK);
            DrawRectangleLines(100, 200, 500, 60, BLACK);
            DrawRectangle(100, 200, x, 60, BLUE);
            DrawText("LOADING...", 300, 215, 20, GREEN);

        EndDrawing();

        if (*arg->counter == arg->size_x){
            CloseWindow();
        }

    }

    return 0;
}



int main(int argc, char *argv[])
{

    int n_vizinhos;
    char *path_train_csv;
    char *path_test_csv;
    int n_threads = 100; // padrão caso não seja passado nada

//---------------------------------------SEPARA CADA ARGUMENTO NO TERMINAL -------------------------------------

    for (int i = 1; i < argc; i+=2){
       switch (argv[i][1])
       {
        case 'k':
        n_vizinhos = atoi(argv[i+1]);
        break;

        case 'd':
        path_train_csv = argv[i+1];
        break;

        case 't':
        path_test_csv = argv[i+1];
        break;

        case 'n':
        n_threads = atoi(argv[i+1]);
       
       default:
           break;
       }
    }

    printf("%s \n", path_train_csv);
    printf("%s \n", path_test_csv);
    printf("%d \n", n_vizinhos);
    printf("%d \n", n_threads);


// --------------------------------------------------------------------------------------------------------------
//                                        TRAIN 
// --------------------------------------------------------------------------------------------------------------




// -------------------------ABRE ALGUNS CSV'S E DEFINE ALGUMAS VARIÁVEIS: TRAIN -----------------------------------------

    // Abre dataset de treinamento
    t_data *data_train = read_csv(path_train_csv);    
    // Treina modelo
    t_knn_classifier *knn_model = knn_fit(data_train, n_vizinhos);
    // Aloca vetor para armazenar a predição
    long *pred_train = malloc(sizeof(long) * data_train->n_rows);



// ----------------------- PASSA ARGUMENTOS PARA AS THREADS FAZEREM AS PREDIÇÕES: TRAIN ---------------------------     

    // Para cada registro (linha da matriz) de treinamento, faz a predição

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    barrier *lista_train = malloc(sizeof(barrier)*data_train->n_rows);
    lista_train->mutex = &mutex;
    lista_train->sem_barrier = malloc(sizeof(sem_t));

    long *counter = malloc(sizeof(long));
    *counter = 0;

    pthread_t *tids_train = malloc(sizeof(pthread_t)*data_train->n_rows);

    sem_init(lista_train->sem_barrier, 0, n_threads); //valor maximo de semaforos rodando 

//----------------------- CRIA STRUCT COM N DE THREADS PARA CRIAR E CONTADOR : TRAIN -----------------------------

    loading *train_creating = malloc(sizeof(loading));
    train_creating->size_x = data_train->n_rows;
    train_creating->counter = counter;
    char nomeTrain[5] = "TRAIN";
    strcpy(train_creating->nome, nomeTrain);

// ----------------------- CRIA THREAD QUE IRÁ MOSTRAR O LOADING DO PROCESSO DE CRIAÇÃO DAS THREADS: TRAIN ----------    

    pthread_t *tid_desenha = malloc(sizeof(pthread_t));
    pthread_create(tid_desenha, NULL, thread_desenha_create, train_creating);


    for (long i = 0; i < data_train->n_rows; i++){
        *counter+=1;
        lista_train[i].mutex = &mutex;
        lista_train[i].sem_barrier = lista_train->sem_barrier;
        lista_train[i].num = i;
        lista_train[i].data = data_train->mat[i];
        lista_train[i].knn_model = knn_model;
        lista_train[i].pred_result = &pred_train[i];
        lista_train[i].counter = counter;
        pthread_create(&tids_train[i], NULL, thread_barreira, &lista_train[i]);
    }

// ------------------------- RODO A THREAD QUE MOSTRA O LOADING DO PROCESSO DE CRIAÇÃO DE THREADS: TRAIN -------------    

    pthread_join(*tid_desenha, NULL);


// ------------------------CHAMA THREAD PARA FAZER O LOADING DO PROCESSO DE EXECUÇÃO : TRAIN ----------------------

    *counter = 0;


    loading *train_loading = malloc(sizeof(loading));
    train_loading->size_x = data_train->n_rows;
    train_loading->counter = counter;
    strcpy(train_loading->nome, nomeTrain);


    // pthread_t *tid_desenha = malloc(sizeof(pthread_t));
    pthread_create(tid_desenha, NULL, thread_desenha_run, train_loading);


    for (long i = 0; i < data_train->n_rows; i++){
        *counter+=1;
        // printf("VAMOS VER O I %ld\n", i);
        pthread_join(tids_train[i], NULL);
    }

    pthread_join(*tid_desenha, NULL);

// --------------------- LIMPA ALGUNS ESPAÇOS ALOCADOS E SEMÁFORO UTILIZADO --------------------------------------    


    sem_destroy(lista_train->sem_barrier);
    free(lista_train);
    free(tids_train);
    free(train_creating);
    free(train_loading);



    printf("Accuracy score train:\t%.3f\n", accuracy_score(get_target(data_train), pred_train, data_train->n_rows));


//-------------------------------------------------------------------------------------------------------------------
//                                                 TEST
//-------------------------------------------------------------------------------------------------------------------



// -------------------- ABRE ALGUNS CSV'S E DEFINE ALGUMAS VARIÁVEIS: TEST ----------------------------------------------------------------

    // Abre os dados de teste
    t_data *data_test = read_csv(path_test_csv);
    long *pred_test= malloc(sizeof(long) * data_test->n_rows);


// ----------------------- PASSA ARGUMENTOS PARA AS THREADS FAZEREM AS PREDIÇÕES: TEST --------------------------------     
    
    barrier *lista_test = malloc(sizeof(barrier)*data_test->n_rows);
    lista_test->mutex = &mutex;
    lista_test->sem_barrier = malloc(sizeof(sem_t));

    pthread_t *tids_test = malloc(sizeof(pthread_t)*data_test->n_rows);

    *counter = 0;


    sem_init(lista_test->sem_barrier, 0, n_threads); //valor maximo de semaforos rodando 

//----------------------- CRIA STRUCT COM N DE THREADS PARA CRIAR E CONTADOR : TEST -----------------------------


    loading *test_creating = malloc(sizeof(loading));
    test_creating->size_x = data_test->n_rows;
    test_creating->counter = counter;
    char nomeTest[5] = "TEST";
    strcpy(test_creating->nome, nomeTest);

    // pthread_t *tid_desenha = malloc(sizeof(pthread_t));
    pthread_create(tid_desenha, NULL, thread_desenha_create, test_creating);


    for (long i = 0; i < data_test->n_rows; i++){
        *counter += 1;
        lista_test[i].mutex = &mutex;
        lista_test[i].sem_barrier = lista_test->sem_barrier;
        lista_test[i].num = i;
        lista_test[i].data = data_test->mat[i];
        lista_test[i].knn_model = knn_model;
        lista_test[i].pred_result = &pred_test[i];
        lista_test[i].counter = counter;
        pthread_create(&tids_test[i], NULL, thread_barreira, &lista_test[i]);
    }

// ------------------------- RODO A THREAD QUE MOSTRA O LOADING DO PROCESSO DE CRIAÇÃO DE THREADS: TEST -------------    


    pthread_join(*tid_desenha, NULL);

// ------------------------CHAMA THREAD PARA FAZER O LOADING DO PROCESSO DE EXECUÇÃO : TEST ----------------------

    *counter = 0;


    loading *test_loading = malloc(sizeof(loading));
    test_loading->size_x = data_test->n_rows;
    test_loading->counter = counter;
    strcpy(test_loading->nome, nomeTest);

    pthread_create(tid_desenha, NULL, thread_desenha_run, test_loading);


    for (long i = 0; i <data_test->n_rows; i++){
        *counter+=1;
        pthread_join(tids_test[i], NULL);
    }

    pthread_join(*tid_desenha, NULL);

// --------------------- LIMPA ALGUNS ESPAÇOS ALOCADOS E SEMÁFORO UTILIZADO --------------------------------------    


    sem_destroy(lista_test->sem_barrier);
    free(lista_test);
    free(tids_test);
    free(test_creating);
    free(test_loading);
    free(tid_desenha);
// --------------------------- IMPRIME RESULTADOS CALCULADOS --------------------------------------------------

    printf("Accuracy score test:\t%.3f\n", accuracy_score(get_target(data_test), pred_test, data_test->n_rows));

    char res1[300];
    char res2[300];
    sprintf(res1, "Accuracy score train:\t%.3f\n", accuracy_score(get_target(data_train), pred_train, data_train->n_rows));
    sprintf(res2, "Accuracy score test:\t%.3f\n", accuracy_score(get_target(data_test), pred_test, data_test->n_rows));

// -------------------- DESENHA JANELA FINAL MOSTRANDO ACURÁCIAS ---------------------------------------------

    InitWindow(800, 450, "Results of accuracy :)");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(res1, 255, 150, 20, BLACK);
            DrawText(res2, 255, 100, 20, BLACK);
        EndDrawing();        
    }

    CloseWindow();



    return 0;

}