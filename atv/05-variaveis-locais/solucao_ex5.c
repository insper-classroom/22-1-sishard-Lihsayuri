int ex5_solucao () {
    // implemente sua resposta aqui.
    // TODO: conserte valor de retorno e argumentos, se for necessário
    int var1 = 1;
    int ebx = 0;

    while (var1 > 0){
        int retval = 0;
        scanf("%d", &var1);

        retval = var1;
        if (retval > 0){
            ebx+= retval;
        }
    }

    return ebx;

}


