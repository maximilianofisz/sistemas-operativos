#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]){
    int dato = 0;
    pid_t pid = fork();
    //si no hay error, pid vale 0 para el hijo
    //y el valor del process id del hijo para el padre
    if (pid == -1) exit(EXIT_FAILURE);
    //si es -1, hubo un error
    else if (pid == 0) {
        for (int i=0; i< 3; i++) {
            dato++;
            printf("Dato hijo: %d\n", dato);
        }
    }
    else {
        for (int i=0; i< 3; i++) {
            printf("Dato padre: %d\n", dato);
        }
    }
    exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}


// Teoria 1:
// Cuando el hijo empieza a modificar dato (linea 15), ahi se hace la copia de la memoria
// y el hijo va printeando 1,2,3 mientras que el padre printea 0 porque en su copia no se modifica
