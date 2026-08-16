#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void ping() {
    printf("PING (%d)\n", getpid());
}

void pong() {
    printf("PONG (%d)\n", getpid());
    kill(getppid(), SIGUSR1);
}

void waitPong() { }


int child() {
    signal(SIGUSR1, pong);
    while (1) pause();
}


int main(int argc, char *argv[]) {

    signal(SIGUSR1, waitPong);

    pid_t pid = fork();
    if (pid == 0) child();

    else {
        char shouldContinueRunning = 'y';
        while(shouldContinueRunning == 'y') {
            for (int i = 0; i < 3; i++) {
                ping();
                kill(pid, SIGUSR1);
                pause();
            }

            printf("¿Querés continuar? [(s/y)/n]: ");
            scanf(" %c", &shouldContinueRunning);  // Espacio antes de %c para limpiar el buffer
        }
        kill(pid, SIGKILL); // Enviamos la señal SIGKILL al hijo
        wait(NULL); // Esperamos a que el hijo termine

        
    }
    exit(EXIT_SUCCESS);
 }