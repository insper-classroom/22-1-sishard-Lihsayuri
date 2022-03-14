/* Implementar função solucao_ex1 */

// rdi = a
// rsi = b
// rdx = c
// rcx = d

int solucao_ex1(long a, long b, long c, long d){
    long retval;
    retval = b*1 + a;
    d = c*4 + retval;
    a *= a;
    retval = 2*b + a;
    c+= retval;

    return (int) (d >= c);
} 