#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

//ssize t send(int s, void *buf, size t len, int flags);
//ssize t recv(int s, void *buf, size t len, int flags);

void Proceso2() { // Cliente

    sleep(1);

    int num = 0;

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket"); // de alguna manera este nombre es una suerte de "direccion??"

    printf("Registrando Socket del servidor en cliente\n");
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    printf("Conectando cliente al servidor\n");
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        printf("Fallo la conexion al servidor\n");
        exit(1);
    }

    while (num < 3) {
        recv(server_socket, &num, sizeof(num), 0);
        printf("Soy el cliente y estoy recibiendo el valor %d\n", num);
        num++;
        send(server_socket, &num, sizeof(num), 0);
        printf("Soy el cliente y estoy enviando el valor %d\n", num);
    }

    close(server_socket);
    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) { // Servidor
    printf("0\n");
    int num = 0;

    pid_t pid = fork(); // separamos P1 y P2
    if (pid == 0) Proceso2(); // Proceso del cliente

    printf("1\n");
    struct sockaddr_un server_addr;
    socklen_t  slen = sizeof(server_addr);
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);
    server_addr.sun_family = AF_UNIX;

    printf("2\n");
    struct sockaddr_un client_addr;
    socklen_t clen = sizeof(client_addr);

    printf("Creando Socket del servidor\n");
    int socket_server = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Bindeando Socket del servidor\n");
    bind(socket_server, (struct sockaddr *) &server_addr, slen);
    printf("Servidor escuchando\n");
    listen(socket_server, 1);


    // Proceso principal del servidor
    int client_socket = accept(socket_server, (struct sockaddr *) &client_addr, &clen); // Aceptamos la conexion del cliente
    while (num < 3) {
        send(client_socket, &num, sizeof(num), 0);
        printf("Soy el servidor y estoy enviando el valor %d\n", num);
        recv(client_socket, &num, sizeof(num), 0);
        printf("Soy el servidor y estoy recibiendo el valor %d\n", num);
    }
        
    close(client_socket);
    waitpid(pid, NULL, 0);

    unlink("unix_socket");
    exit(EXIT_SUCCESS);
}