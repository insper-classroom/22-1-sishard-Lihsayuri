#include <stdlib.h>
#include "vec_int.h"

typedef struct _vec_int {
    int *data;
    int size;
    int capacity;
} vec_int;


vec_int *vec_int_create() {
    vec_int *v = malloc(sizeof(vec_int)); 
    v->capacity = 2;
    v->size = 0;
    v->data = malloc(v->capacity * sizeof(int));
    return v;
}

void vec_int_destroy(vec_int **_v) {
    free((*_v)->data);
    free(*_v);
    (*_v) = NULL;
}

int vec_int_size(vec_int *v) {
    return v->size;
}

int vec_int_at(vec_int *v, int pos, int *val) {
    if (pos < v->size && pos >= 0){
        *val = v->data[pos]; 
        return 1;
    } 

    return 0;

}

void troca(int v[], int i, int j) {
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


int vec_int_insert(vec_int *v, int pos, int val) {
    if (pos >= 0 && pos <= v->size){
        if (v->size >= v->capacity){
            v->capacity *= 2;
            v->data = realloc(v->data, v->capacity * sizeof(int));
        }

        for (int i = v->size; i > pos ; i--){
            v->data[i] = v->data[i-1];
        }

        v->data[pos] = val;
        v->size+= 1;

        return 1;
    }

    return 0;
}

int vec_int_remove(vec_int *v, int pos) {
    if (pos >= 0 && pos < v->size){
        if (v->size <= v->capacity/4){
            v->capacity /= 2;
            v->data = realloc(v->data, v->capacity * sizeof(int));
        }


        for (int i = pos; i < v->size ; i++){
            v->data[i] = v->data[i+1];
        }

        v->size-= 1;

        return 1;
    }

    return 0;
}

