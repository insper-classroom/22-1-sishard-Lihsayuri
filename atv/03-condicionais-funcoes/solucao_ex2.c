long vezes2(long a);

// rdi = a
// rsi = b
// rbx = c


long solucao_ex2(long a, long b){
    long c = a;
    a = b;
    long retval = vezes2(a);
    if (retval >  c){
        c+= c;
    }
    
    return (retval + c);
}