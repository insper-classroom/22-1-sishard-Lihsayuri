#include <stdio.h>
#include <stdlib.h>
#include "macros_correcoes.h"


/* Recebe duas strings e concatena em uma terceira */
char *mystrcat(char *str1, char *str2) {
    char *new_string = malloc((strlen(str1)*sizeof(char)) + (strlen(str2)*sizeof(char)) + 1);

    for (int i = 0; i < (strlen(str1) + strlen(str2)); i++){
        if (i < strlen(str1)){
            new_string[i] = str1[i];
        } else {
            new_string[i] = str2[i - strlen(str1)];
        }
    }

    new_string[(strlen(str1) + strlen(str2))] = '\0';

    return new_string;
}

int main(int argc, char *argv[]) {
    # define teste1 "str1"
    # define teste2 "str2"
    # define teste3 "0 str 3"


    char *res = mystrcat(teste1, teste2);
    assertEquals("teste 1", strcmp(res, teste1 teste2), 0);
    free(res);

    res = mystrcat(teste1, teste1);
    assertEquals("teste 2", strcmp(res, teste1 teste1), 0);
    free(res);

    res = mystrcat(teste1, teste3);
    assertEquals("teste 3", strcmp(res, teste1 teste3), 0);
    free(res);

    res = mystrcat(teste3, teste1);
    assertEquals("teste 4", strcmp(res, teste3 teste1), 0);
    free(res);

    res = mystrcat("", teste2);
    assertEquals("teste vazio 1", strcmp(res, teste2), 0);
    free(res);

    res = mystrcat(teste1, "");
    assertEquals("teste vazio 2", strcmp(res, teste1), 0);
    free(res);

    res = mystrcat("", "");
    assertEquals("teste vazio 3", strcmp(res, ""), 0);
    free(res);

    printSummary

    return 0;
}
