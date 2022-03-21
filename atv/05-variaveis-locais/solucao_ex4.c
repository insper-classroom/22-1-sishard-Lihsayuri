int ex4_solucao(int d) {
    // implemente sua resposta aqui.
    // TODO: conserte valor de retorno e argumentos, se for necessário

    // d é ebx
    int ebx = d;

    int var1;
    int var2;
    scanf("%d %d", &var2, &var1);

    int retval = var2 + 2*var1;
    return (retval+d);
}

// rbx é local

// %rdi
// %rsi
// %rdx
// %rcx
// %r8
// %r9
