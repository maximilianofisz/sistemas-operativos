#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int server_fd;
struct pollfd fds[6];


void cleanup(int sig) {
    printf("\nCerrando servidor...\n");

    for (int i = 0; i < 6; i++) {
        if (fds[i].fd != -1) {
            close(fds[i].fd);
        }
    }

    close(server_fd);
    unlink("socket");

    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) { // Servidor

    signal(SIGINT, cleanup);

    struct sockaddr_un server_addr;
    int client_fd;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "socket");

    unlink("socket");

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    for (int i = 0; i < 5; i++) { // Registramos a los clientes, todos "desconectados"
        fds[i].fd = -1;
    }

    fds[0].fd = server_fd; // Registramos el servidor
    fds[0].events = POLLIN;

    int nfds = 1;

    // Proceso principal del servidor
    while (1) {
        poll(fds, nfds , -1); // Aca nos quedamos esperando a que pase algo basicamente

        if(fds[0].revents & POLLIN) { // Caso nuevo cliente
            client_fd = accept(server_fd, NULL, NULL);
            printf("Nuevo cliente!\n");

            for (int i = 1; i < 5; i++) { // Recorremos el registro y buscamos un slot libre
                if(fds[i].fd == -1) {
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;
                    if(i >= nfds) nfds = i + 1;
                    break;
                }
            }
        }

        for (int i = 1; i < 5; i++) { // Caso revisar eventos de los clientes
            if (fds[i].fd == -1) continue; // Subcaso este cliente no esta conectado

            if (fds[i].revents & POLLIN) { // Subcaso este cliente esta conectado y envio algo
                char msg_received[100];
                int n = recv(fds[i].fd, msg_received, sizeof(msg_received), 0); // guardamos lo que se envio en num y n es como el resultado

                if (n <= 0) { // n es como el resultado porque es el tamaño del mensaje recibido (-1 es error y 0 es desconectado)
                    printf("Cliente desconectado!\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                else {
                    msg_received[n] = '\0';
                    printf("Recibí el mensaje: %s", msg_received);
                    // subcaso, voy a difundir el mensaje al resto de clientes
                    for (int j = 1; j < 5; j ++) { //Recorremos el resto de clientes
                        if (i == j) continue; // No me reenvio a mí mismo el msg
                        if (fds[j].fd == -1) continue; // Subcaso este cliente no esta conectado

                        send(fds[j].fd, msg_received, strlen(msg_received), 0);
                    }
                }
            }
        }
    }
}