/* Implemente aqui sua versão em C da função ex5
 * presente no arquivo ex3.o
 * 
 * Chame sua função de ex3_solucao */

// ecx = variavel local
// r8d = variavel local
// rcx = d - parâmetro
// rdi - a - parametro
// rdx = fica com resto;

long ex3_solucao(long a, long b, long d){
    int var1 = 0;
    long var2 = 0;
    long retval;
    while (d < a){
        retval = d;
        if ((retval % b) == 0){
            var2+=d;
        }

        d +=1;
    }

    return var2;

}