#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <signal.h>


void child1() {
    // quiero esperar al mensaje del padre
    while (1) {
        int msg = breceive(getppid());
        bsend(pid2, msg + 1);
    }
}

void child2() {
    // quiero esperar al mensaje del padre con el pid del hermano
    int pid1 = breceive(getppid());
    bsend(getppid(), 0);

    while (1) {
        int msg = breceive(pid1);
        bsend(getppid(), msg + 1);
    }
}

int main(int argc, char *argv[]) {

    pid_t pid2;
    pid_t pid1;

    pid2 = fork(); 
    if (pid2 == 0) child2(); 
    else { 
        pid1 = fork(); 
        if (pid1 == 0) child1();

        bsend(pid2, pid1);
        breceive(pid2);
        
        int msg = 0;
        while (msg < 50) {
            bsend(pid1, msg); // le envio 0 en la primer iteracion al hijo
            msg = breceive(pid2) + 1;
        }
    }


    kill(pid1, SIGKILL); // Enviamos la señal SIGKILL al hijo
    wait(NULL); // Esperamos a que el hijo termine

    kill(pid2, SIGKILL); // Enviamos la señal SIGKILL al hijo
    wait(NULL); // Esperamos a que el hijo termine

    exit(EXIT_SUCCESS);

}