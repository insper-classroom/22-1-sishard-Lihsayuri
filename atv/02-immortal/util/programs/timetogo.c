/*
 PROGRAMA UTIL PARA TESTAR O LAB!

 Funcionalidade: este programa espera TIME_SLEEP segundos e finaliza normalmente

 Para compilar: gcc timetogo.c -o timetogo
 */

#include <unistd.h>

#define TIME_SLEEP 5

int main() {

    sleep(TIME_SLEEP);

    return 0;
}