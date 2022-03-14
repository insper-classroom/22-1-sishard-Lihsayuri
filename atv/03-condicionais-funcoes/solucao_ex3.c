/* Implementar função solucao_ex3 */

// rdi = a
// rsi = b
// rdx = c
// rcx = d
// r8 = e

void solucao_ex3(long a, long b, int *c, int *d, int *e){
    int retval;
    retval = (a < b);
    *c = retval;

    retval = (a == b);
    *d = retval;

    retval = (a > b);
    *e = retval;
}