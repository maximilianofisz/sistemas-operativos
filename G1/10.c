#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int Jorge() {
    write(1, "Soy Jorge\n", 10);

    sleep(1);

    exit(EXIT_SUCCESS);
}

int Jennifer() {
    write(1, "Soy Jennifer\n", 14);

    sleep(1);

    exit(EXIT_SUCCESS);
}

int Julieta() {
    write(1, "Soy Julieta\n", 12);

    sleep(1);
    
    if (fork() == 0) Jennifer();

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    if (fork() == 0) Julieta();

    write(1, "Soy Juan\n", 10);

    sleep(1);

    wait(NULL);
    
    if (fork() == 0) Jorge();

    exit(EXIT_SUCCESS);

}