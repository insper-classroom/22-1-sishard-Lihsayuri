#include <stdio.h>

int level1(int a){
    return ((-5583) == a);
} // tenho que preencher o 'a' que resulta em 1. 



int level3(long a){
    int retval = 3*a;
    if (retval <= 442){
        return 1;
    } if (retval < -442){
        return 0;
    } if ((retval & 1) != 0){
        return 0;
    } if (retval != 0){
        return 1;
    }

}


// rdx sempre guarda o resto
int level4(long a){
    long retval = a;
    int b = 10;
    
    if ((retval % b) == 9){
        return 1;
    }

    return 0;
}


int level5(long a, long b, long *c){
    long retval = 5*a;
    retval += 81 + 4*b;

    if (((20492006 == retval) && (b != a)) == 0){
        *c = 1;
    } else{
        *c = 0;
    }

    char al =  (retval > 55875); 
    char dl = (a < -14);

    return (al && dl);
}

void teste(long *s, long d, long c, int a, int b){
    d = 8+ *s;
    c = 12 + *s;
    b = 3;
    a = 10;
}


int level6(){
    int d; //0x4(%rsp),%rcx
    int c; //0x8(%rsp),%rdx
    int b; //0xc(%rsp),%rsi

    //long a = "%d %d %d";



    scanf("%d %d %d", &d, &c, &b);

    int var1 =  b;
    int var2 = c;

    int retval = var1+var2;

    int var3 = d;

    retval+= var3;

    // soma b + c + d

    if (var1 <= 11){
        retval = 0;
    }
    if (var2 <= 11){
        retval = 0;
    }

    if (var3 <= 11){
        retval = 0;
    } 

    if (retval <= 39){
        retval = 1;
    }

    return retval;
}


// sar é um shift e diz quantas vezes vai shiftar para a direita.

int level7(){
    long b;
    scanf("%ld", &b);

    int retval = 0;
    int d = 0;


    while (retval <= 11){
        long c = retval;
        d+= c;
        retval+=1;
    }

    // retval vai somar 12; e d também

    long c = 6148914691236517206;
    retval = d;
    retval *= c; // dá 44 vai pro rax (menos significativos). e o rdx (mais significativos) armazena 22 

    //long rdx tBM armazena valor da divisão;
    // ele vai shiftar para a direita e aí vê se é positivo 
    d >>= 63;
    c -= d;

    char al =  (b == c);

    return (int) al;
}


int soma (int *v, int n){
    int soma;
    for (int i = 0; i < n; i++){
        soma+= v[i];
    }

    return soma;
}

int soma_errada (int *v, int n){
    for (int i = 1; i < n; i++){
        v[i]+= v[i-1];
    }

    return v[n-1];
}

int level8(){
    long b;
    scanf("%u", &b); // unsigned int, ou seja, positivo!
    int c = 1;
    int retval = b;

    // below or equal (menor ou igual para unsigned)/ fazer 16 vezes o loop
    // o 2 tem que ser elevado a que número para dividir 16 vezes = 2^16
    while (retval > 1){
        retval /= 2;
        c+=1;
    }

    return (17 == c);
}



func3(int vec[], int n){
    int c = 0;
    int soma = 0;

    while (c < n){
        soma += vec[c];
        c+=1;
    }

    return soma;
}

int func2(int vec[], int n){
    int retval = 0;

    while (retval < n){
        if (vec[retval] < 0){
            return 1;
        }
        retval+=1;

    }
}

int func1(int vec[], int b){
    int i = 0;
    int edx = b-1;

    while (edx > i){
    // se é uma lista de longs ele começa no 1
        if (vec[i] > vec[i+1]){
            return 0;
        }
        i+=1;
    }
    return 1;
} 

int level9(int b, long *v)
{
    // rbx adota como uma variável normal
    int ebx = 0;
    while (ebx <= 9){
        long c = ebx;
        long *b = &v[ebx];
    
        scanf("%ld", &b);

        ebx+=1;
    }

    b = 10;
    int eax_func3 = func3(&v, b);

    if (eax_func3 <= 10){
        return 0;
    } // minha soma tem que ser MAIOR DO QUE 10;

    b = 10;
    int eax_func2 = func2(&v,b);
    // vai ter que ter alguém negativo

    if (eax_func2 == 0){
        return eax_func2;
    }

    b = 10;
    int eax_func1 = func1(&v, b); // vai ter que ser crescente
    if (eax_func1 == 0){
        return eax_func1;
    }

    return 1;
}

// int level10(){
//     long b;

//     scanf("%d", &b);

//     int c = 0;

//     //al é um byte
//     long retval = 1;


//     while (retval != 0){
//         retval = c;
//         char d[7] = "LÍVIA"; // pega o valor número pra string

//         retval = d[retval];
//         b-= retval;
//         c+=1;
//     }

//     printf()

//     // 76 214 141 86 73 65 +++++++ 7

//     return b == 7;

// }


int main(){
    char d[7] = "LÍVIA";
    for (int i = 0; i < 7; i++){
        printf("%u ", d[i]);
    }

    return 0;
}
