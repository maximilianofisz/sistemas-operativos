#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int server_fd;
struct pollfd fds[2];

void cleanup(int sig) {
    printf("\nCerrando cliente...\n");

    close(server_fd);

    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) { // Servidor

    signal(SIGINT, cleanup);

    char msg_received[100];
    char msg_sent[100];

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "socket"); // de alguna manera este nombre es una suerte de "direccion??"

    printf("Registrando Socket del servidor en cliente\n");
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    // 1 = teclado
    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;

    printf("Conectando cliente al servidor\n");
    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        printf("Fallo la conexion al servidor\n");
        exit(1);
    }

    while (1) {
        poll(fds, 2, -1); // polleamos aca tambien por cambios en el socket y en el teclado

        // mensaje del servidor
        if (fds[0].revents & POLLIN) {
            int n = recv(server_fd, &msg_received, sizeof(msg_received) - 1, 0);
            printf("Recibi %d bytes\n", n);
            msg_received[n] = '\0'; // como recv nos devuelve el tamaño del mensaje recibido le agregamos el final
            printf("Se recibio el msg: %s\n", msg_received);
        }

        // input del usuario
        if (fds[1].revents & POLLIN) {
            fgets(msg_sent, 100, stdin);
            send(server_fd, &msg_sent, strlen(msg_sent), 0);
        }
    }

}