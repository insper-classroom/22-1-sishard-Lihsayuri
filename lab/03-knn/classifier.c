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


// -k n_vizinhos: informa o argumento do classificador, números de vizinhos a serem considerados
// -d path_train_csv: informa o path para o arquivo CSV de treinamento
// -t path_test_csv: informa o path para o arquivo CSV de teste

typedef struct args{
    sem_t *sem_barrier;
    pthread_mutex_t *mutex;
    long *pred_result;
    t_knn_classifier *knn_model;
    double *data;
    int num;
} barrier;

void *thread_barreira(void *args) {
    barrier *arg = (barrier *)args;
    sem_wait(arg->sem_barrier);

    //pthread_mutex_unlock(arg->mutex);
    //printf("Antes da barreira\n");
    //pthread_mutex_unlock(arg->mutex);

    pthread_mutex_lock(arg->mutex);
    printf("Esse é o numero da thread: %d \n", arg->num);
    pthread_mutex_unlock(arg->mutex);
    //sleep(2);

    *arg->pred_result = knn_predict(arg->knn_model, arg->data);
    
    // pred_train[i] = knn_predict(knn_model, data_train->mat[i]);

    //pthread_mutex_lock(arg->mutex);
    //printf("Depois da barreira, essa é a thread: %d\n", arg->num);
    //pthread_mutex_unlock(arg->mutex);

    sem_post(arg->sem_barrier);

    return NULL;
}

int main(int argc, char *argv[])
{

    int n_vizinhos;
    char *path_train_csv;
    char *path_test_csv;
    int n_threads;

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

    for (int i = 0; i < data_train->n_rows; i++){
        lista_train[i].mutex = &mutex;
        lista_train[i].sem_barrier = lista_train->sem_barrier;
        lista_train[i].num = i;
        lista_train[i].data = data_train->mat[i];
        lista_train[i].knn_model = knn_model;
        lista_train[i].pred_result = &pred_train[i];
        pthread_create(&tids_train[i], NULL, thread_barreira, &lista_train[i]);
    }

    for (int i = 0; i < data_train->n_rows; i++){
        pthread_join(tids_train[i], NULL);
    }

    sem_destroy(lista_train->sem_barrier);


    printf("Accuracy score train:\t%.3f\n", accuracy_score(get_target(data_train), pred_train, data_train->n_rows));

// -------------------- Abre alguns csvs  e define algumas variáveis: TRAIN ----------------------------------------------------------------

    // Abre os dados de teste
    t_data *data_test = read_csv(path_test_csv);
    long *pred_test= malloc(sizeof(long) * data_test->n_rows);


// ----------------------- Cria mutex e passa argumentos para as threads fazerem as predições: TRAIN ---------------------------     

    
    barrier *lista_test = malloc(sizeof(barrier)*data_test->n_rows);
    lista_test->mutex = &mutex;
    lista_test->sem_barrier = malloc(sizeof(sem_t));

    pthread_t *tids_test = malloc(sizeof(pthread_t)*data_test->n_rows);

    sem_init(lista_test->sem_barrier, 0, n_threads); //valor maximo de semaforos rodando 

    for (int i = 0; i < data_test->n_rows; i++){
        lista_test[i].mutex = &mutex;
        lista_test[i].sem_barrier = lista_test->sem_barrier;
        lista_test[i].num = i;
        lista_test[i].data = data_test->mat[i];
        lista_test[i].knn_model = knn_model;
        lista_test[i].pred_result = &pred_test[i];
        pthread_create(&tids_test[i], NULL, thread_barreira, &lista_test[i]);
    }

    for (int i = 0; i <data_test->n_rows; i++){
        pthread_join(tids_test[i], NULL);
    }

    sem_destroy(lista_test->sem_barrier);

    printf("Accuracy score test:\t%.3f\n", accuracy_score(get_target(data_test), pred_test, data_test->n_rows));

    return 0;

}