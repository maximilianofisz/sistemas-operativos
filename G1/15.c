#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <signal.h>

//int dup2(int oldfd, int newfd): si oldfd y newfd son
//distintos, primero se elimina la referencia al objeto apuntado por
//newfd, y luego se apunta newfd al mismo objeto que oldfd.

// int close(int d): Cierra para el proceso actual el descriptor d pasado
//por par´ametro.


void child_ls(int fds[]) {
    dup2(fds[1], 1); // hago que el STDOUT apunte al principio del pipe
    close(fds[0]); // Cerramos los fd no usados
    execl("/bin/ls", "ls", "-al", NULL);
    exit(EXIT_SUCCESS);
}

void child_wc(int fds[]) {
    dup2(fds[0], 0); // hago que el STDIN apunte al final del pipe
    close(fds[1]); // Cerramos los fd no usados
    execl("/bin/wc", "wc", "-l", NULL);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    // este programa tiene que replicar "ls -al | wc -l" que cuenta los archivos que hay en el wd + 1
    int fds[2];
    pipe(fds); // fds[0] se lee, fds[1] se escribe
    
    pid_t pid_ls;
    pid_t pid_wc;
    
    pid_ls = fork();
    if (pid_ls == 0) child_ls(fds);
    else {
        pid_wc = fork();
        if (pid_wc == 0) child_wc(fds);
    }

    close(fds[0]);
    close(fds[1]);
    
    waitpid(pid_ls, NULL, 0);
    waitpid(pid_wc, NULL, 0);

    exit(EXIT_SUCCESS);
}