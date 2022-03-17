
/* Implemente aqui sua versão em C da função soma_n
 * presente no arquivo ex2.o
 *
 * Chame sua função de soma_n_solucao */


long soma_n_solucao(int a){
    long c = 0;
    int retval = 0;
    
    while (retval < a){
        long d = retval;
        c+=d;
        retval+=1;
    }

    return c;

}
