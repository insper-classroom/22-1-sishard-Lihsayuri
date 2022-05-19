#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int num_barrier = 0;

typedef struct args{
    sem_t *sem_barrier;
    pthread_mutex_t *mutex;
} barrier;


void *thread_barreira(void *args) {
    barrier *arg = (barrier *)args;
    printf("Antes da barreira\n");

    if (num_barrier != 3){
        pthread_mutex_lock(arg->mutex);
        num_barrier ++;
        pthread_mutex_unlock(arg->mutex);
        sem_wait(arg->sem_barrier);
    } else {
        for (int i = 0; i < 3; i++){
            sem_post(arg->sem_barrier);
        }
    }

    printf("Depois da barreira\n");

    return NULL;
}

int main() {
    barrier *args = malloc(sizeof(barrier));

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    args->mutex = &mutex;

    args->sem_barrier = malloc(sizeof(sem_t));

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    sem_init(args->sem_barrier, 0, 0);


    int error1 = pthread_create(&tid1, NULL, thread_barreira, args);
    int error2 = pthread_create(&tid2, NULL, thread_barreira, args);
    int error3 = pthread_create(&tid3, NULL, thread_barreira, args);
    int error4 = pthread_create(&tid4, NULL, thread_barreira, args);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    sem_destroy(args->sem_barrier);
    
        
    return 0;
}
