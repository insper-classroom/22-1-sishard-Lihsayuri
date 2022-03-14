// TODO implemente aqui uma funcao chamada "solucao"

// edi = int a
// rsi = long b
// rdx = long c

int solucao(int a, long b, int *c){
    a+= a;
    int retval = b*4 + b;
    retval += a;
    *c = retval;
    retval = b*2 + b;
    retval += a;
    return retval;
}
