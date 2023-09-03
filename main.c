#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    /*
        COMENTADO POR FELIPE CRAVERO
        EN ESTA SECCION SE DEFINEN VARIABLE UTILES
    */
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    // ACA SE DEFINE UNA VARIABLE PRACTICA PARA NO TENER QUE ANDAR LLAMANDO SIZEOF
    int addrlen = sizeof(address);

    char *hello = "Hello from server";

    // USANDO LA FUN SOCKET DEBAJO LA DEFINICION DE socket
    // socket(domain, type, protocol); de vuelve -1 si hay un error
    // ESTE SOCKET SIRVE PARA ESCUCHAR LOS CLIENTES, MAS ADELANTE
    // SE CREAN NUEVOS SOCKETS PARA PODER ESCRIBIR LA RESPUESTA
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    // sin_family SIRVE PARA DEFINIR LA FAMILIA DE LA DIRECCION
    // AF_INET SIGNIFICA IPV4
    address.sin_family = AF_INET;
    // s_addr SIRVE PARA DEFINIR LA DIRECCION DE EL SOCKET
    address.sin_addr.s_addr = INADDR_ANY;
    // sin_port SIRVE PARA DEFINIR EL NUMERO DE PORT
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    // BIND SIRVE PARA ASIGNAR LAS "DIRECCIONES", TOMANDO server_fd
    // PARA ASIGNARLE LA STRUCT ADDRESS
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    // LISTEN SIRVE PARA CONFIGURAR EL SOCKET DE SOLICITUDES
    // EL SEGUNDO PARAMETRO DETEMINA CUANTOS REQUESTS
    // ESCUCHARA EL SOCKET ANTES DE DESECHAR LOS ENTRANTES
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        // ACCEPT SIRVE PARA DEFINIR QUE EL SOCKET PREVIAMENTE DEFINIDO
        // COMO LISTENER,
        // EL PRIMER PARAMETRO TOMA EL SOCKET, EL SEGUNDO TOMA UNA STRUCT ADDRESS
        // PARA GUARDAR INFORMACION DE EL SOCKET QUE ESTA HACIENDO LA PETICION
        // Y EL ULTIMO
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}