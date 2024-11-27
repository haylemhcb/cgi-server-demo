#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, sizeof(buffer) - 1);
    
    // Imprimir la solicitud en la consola
    printf("Request:\n%s\n", buffer);

    // Respuesta HTTP básica
    const char *header = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n";
    const char *body = "<html><body><h1>Hello, CGI!</h1></body></html>";

    // Enviar la cabecera y el cuerpo de la respuesta
    write(client_socket, header, strlen(header));
    write(client_socket, body, strlen(body));

    // Cerrar el socket del cliente
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Crear el socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Enlazar el socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al enlazar el socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(server_socket, 5) < 0) {
        perror("Error al escuchar");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor CGI escuchando en el puerto %d...\n", PORT);

    while (1) {
        // Aceptar una conexión entrante
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error al aceptar la conexión");
            continue;
        }

        // Manejar la conexión del cliente
        handle_client(client_socket);
    }

    // Cerrar el socket del servidor
    close(server_socket);
    return 0;
}
