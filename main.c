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

        basado en :
        https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa#id_token=eyJhbGciOiJSUzI1NiIsImtpZCI6ImM3ZTExNDEwNTlhMTliMjE4MjA5YmM1YWY3YTgxYTcyMGUzOWI1MDAiLCJ0eXAiOiJKV1QifQ.eyJpc3MiOiJodHRwczovL2FjY291bnRzLmdvb2dsZS5jb20iLCJhenAiOiIyMTYyOTYwMzU4MzQtazFrNnFlMDYwczJ0cDJhMmphbTRsamRjbXMwMHN0dGcuYXBwcy5nb29nbGV1c2VyY29udGVudC5jb20iLCJhdWQiOiIyMTYyOTYwMzU4MzQtazFrNnFlMDYwczJ0cDJhMmphbTRsamRjbXMwMHN0dGcuYXBwcy5nb29nbGV1c2VyY29udGVudC5jb20iLCJzdWIiOiIxMTMxNTQwNjQ0MzU0NzEzMjA1ODMiLCJlbWFpbCI6ImZlbGlwZWNyYXZlcm8wNUBnbWFpbC5jb20iLCJlbWFpbF92ZXJpZmllZCI6dHJ1ZSwibmJmIjoxNjkzNzc4MDY1LCJuYW1lIjoiRmVsaXBlIENyYXZlcm8iLCJwaWN0dXJlIjoiaHR0cHM6Ly9saDMuZ29vZ2xldXNlcmNvbnRlbnQuY29tL2EvQUFjSFR0ZkNMODg4SndNWDR4WVBDYkE0eDVZckFJdGpyNkd1QzZqb21GRndUUUVBMHc9czk2LWMiLCJnaXZlbl9uYW1lIjoiRmVsaXBlIiwiZmFtaWx5X25hbWUiOiJDcmF2ZXJvIiwibG9jYWxlIjoiZW4iLCJpYXQiOjE2OTM3NzgzNjUsImV4cCI6MTY5Mzc4MTk2NSwianRpIjoiNDNjNTI0Zjk2ODM5MzQ1NGUxOGMxZjg4NjFhNTdmYzk2N2IyOWI0NCJ9.DbIdJao4thJOAA2bq1Dm9JdxVm5bO6JniWZBndLjSLK9--YeRfdzSSPywLMOynNB5yEyyg-F2_O-Tmp9-y0l7DdkiXVfgXf7wWo2oUK0bNWyWCWDPWFaoaAF_T-ysqkyCRJnfRIIJomxfeH3HZcvuj0AO-wBaGDtTfVCPQEqXuOnD6H0vn_fVsxXZoxjLY4FZKrFmMxvln2JQXO_8F8UaOszBqOnxb6oCwFeaKisjOLi_C8ubQDbOaelrX8C4pwnhhLXvZEvQEOW-NU63x9q3XXXsJ1dP050L7WSMBzRkzcnRY6ivfpOSEHkEZqmQUn_UA5z7TjFKJHSdssg7YVp2g
        links utiles:
        https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_16.html#SEC329
    */
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    // ACA SE DEFINE UNA VARIABLE PRACTICA PARA NO TENER QUE ANDAR LLAMANDO SIZEOF
    int addrlen = sizeof(address);

    FILE *httpDevo = fopen("../index.txt", "r");
    if (httpDevo == NULL)
    {
        printf("cant open file to serve");
        exit(EXIT_FAILURE);
    }
    char indxStringer[370] = {};
    char caracter;
    int contador = 0;
    memset(indxStringer, "\0", sizeof(indxStringer));
    while ((caracter = fgetc(httpDevo)) != EOF && contador < 370 - 1)
    {
        indxStringer[contador] = caracter;
        contador++;
    }
    printf("%s", indxStringer);
    fclose(httpDevo);
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
        // Y EL ULTIMO TOMA LA LONGITUD DE LA STRUCT ADDRESS
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        // ACA SE DEFINE EL BUFFER PARA GUARDAR LA INFORMACION DE EL SOCKET
        char buffer[30000] = {0};
        // ACA SE LLAMA A READ Y WRITE PARA DARLES UN USO IGUAL A COMO SI FUERAN
        // ARCHIVOS PRINTEANDO Y RECIBIENDO LOS DATOS
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, indxStringer, strlen(indxStringer));
        printf("------------------sent %s -------------------\n", indxStringer);
        close(new_socket);
    }
    free(indxStringer);
    return 0;
}