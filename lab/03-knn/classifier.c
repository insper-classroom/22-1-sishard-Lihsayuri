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


int counter = 0;

typedef struct args{
    sem_t *sem_barrier;
    pthread_mutex_t *mutex;
    long *pred_result;
    t_knn_classifier *knn_model;
    double *data;
    int num;
} barrier;


typedef struct relacao_loading{
    long size_x;
    int counter;
} loading; 


// --------------- THREAD BARREIRA PARA ESPERAR TODAS AS THREADS E LIMITAR SUA EXECUÇÃO SIMULTÂNEA EM 1 NÚMERO -------

void *thread_barreira(void *args) {
    barrier *arg = (barrier *)args;
    sem_wait(arg->sem_barrier);

    pthread_mutex_lock(arg->mutex);
    counter++;
    //printf("Esse é o numero da thread: %d \n", arg->num);
    pthread_mutex_unlock(arg->mutex);

    *arg->pred_result = knn_predict(arg->knn_model, arg->data);
    
    sem_post(arg->sem_barrier);

    return NULL;
}

//--------- THREAD PARA DESENHAR UMA JANELA QUE MOSTRA UMA BARRA DE PROGRESSO DA EXECUÇÃO DAS THREADS ----------------

void *thread_desenha(void *args){
    loading *arg = (loading *)args;

    InitWindow(800, 450, "Loading Window");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        int x = ((float)600/arg->size_x)*counter;
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("LOADING...", 255, 150, 20, BLACK);
            DrawRectangleLines(100, 200, 600, 60, BLACK);
            DrawRectangle(100, 200, x, 60, BLUE);

        EndDrawing();

        if (counter == arg->size_x){
            CloseWindow();
        }

    }

    counter = 0;

    return 0;
}



int main(int argc, char *argv[])
{

    int n_vizinhos;
    char *path_train_csv;
    char *path_test_csv;
    int n_threads = 100; // padrão caso não seja passado nada

//----------------------------------------------- Separa cada argumento do terminal -------------------------------------

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

// ------------------------------ Abre alguns csvs  e define algumas variáveis: TRAIN ---------------------------------------

    // Abre dataset de treinamento
    t_data *data_train = read_csv(path_train_csv);    
    // Treina modelo
    t_knn_classifier *knn_model = knn_fit(data_train, n_vizinhos);
    // Aloca vetor para armazenar a predição
    long *pred_train = malloc(sizeof(long) * data_train->n_rows);



// ----------------------- Cria mutex e passa argumentos para as threads fazerem as predições: TRAIN ---------------------------     

    // Para cada registro (linha da matriz) de treinamento, faz a predição

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    barrier *lista_train = malloc(sizeof(barrier)*data_train->n_rows);
    lista_train->mutex = &mutex;
    lista_train->sem_barrier = malloc(sizeof(sem_t));

    pthread_t *tids_train = malloc(sizeof(pthread_t)*data_train->n_rows);

    sem_init(lista_train->sem_barrier, 0, n_threads); //valor maximo de semaforos rodando 


    loading *train_loading = malloc(sizeof(loading));
    train_loading->size_x = data_train->n_rows;
    train_loading->counter = counter;

    for (int i = 0; i < data_train->n_rows; i++){
        lista_train[i].mutex = &mutex;
        lista_train[i].sem_barrier = lista_train->sem_barrier;
        lista_train[i].num = i;
        lista_train[i].data = data_train->mat[i];
        lista_train[i].knn_model = knn_model;
        lista_train[i].pred_result = &pred_train[i];
        pthread_create(&tids_train[i], NULL, thread_barreira, &lista_train[i]);
    }

    pthread_t *tid_desenha = malloc(sizeof(pthread_t));
    pthread_create(tid_desenha, NULL, thread_desenha, train_loading);

    pthread_join(*tid_desenha, NULL);
    for (int i = 0; i < data_train->n_rows; i++){
        pthread_join(tids_train[i], NULL);
    }

    sem_destroy(lista_train->sem_barrier);
    free(lista_train);
    free(tids_train);


    printf("Accuracy score train:\t%.3f\n", accuracy_score(get_target(data_train), pred_train, data_train->n_rows));

// -------------------- Abre alguns csvs  e define algumas variáveis: TEST ----------------------------------------------------------------

    // Abre os dados de teste
    t_data *data_test = read_csv(path_test_csv);
    long *pred_test= malloc(sizeof(long) * data_test->n_rows);


// ----------------------- Cria mutex e passa argumentos para as threads fazerem as predições: TEST ---------------------------     
    
    barrier *lista_test = malloc(sizeof(barrier)*data_test->n_rows);
    lista_test->mutex = &mutex;
    lista_test->sem_barrier = malloc(sizeof(sem_t));

    pthread_t *tids_test = malloc(sizeof(pthread_t)*data_test->n_rows);

    sem_init(lista_test->sem_barrier, 0, n_threads); //valor maximo de semaforos rodando 

    loading *test_loading = malloc(sizeof(loading));
    test_loading->size_x = data_test->n_rows;
    test_loading->counter = counter;


    for (int i = 0; i < data_test->n_rows; i++){
        lista_test[i].mutex = &mutex;
        lista_test[i].sem_barrier = lista_test->sem_barrier;
        lista_test[i].num = i;
        lista_test[i].data = data_test->mat[i];
        lista_test[i].knn_model = knn_model;
        lista_test[i].pred_result = &pred_test[i];
        pthread_create(&tids_test[i], NULL, thread_barreira, &lista_test[i]);
    }


    pthread_create(tid_desenha, NULL, thread_desenha, test_loading);
    pthread_join(*tid_desenha, NULL);

    for (int i = 0; i <data_test->n_rows; i++){
        pthread_join(tids_test[i], NULL);
    }

    sem_destroy(lista_test->sem_barrier);
    free(lista_test);
    free(tids_test);

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


// -lraylib