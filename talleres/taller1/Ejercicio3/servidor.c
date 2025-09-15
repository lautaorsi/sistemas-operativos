#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int calcular(const char *expresion) {
    int num1, num2, resultado;
    char operador;

    // Usamos sscanf para extraer los dos números y el operador de la expresión
    if (sscanf(expresion, "%d%c%d", &num1, &operador, &num2) != 3) {
        printf("Formato incorrecto\n");
        return 0;  // En caso de error, retornamos 0.
    }

    // Realizamos la operación según el operador
    switch (operador) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
            } else {
                printf("Error: División por cero\n");
                return 0;  // Si hay división por cero, retornamos 0.
            }
            break;
        default:
            printf("Operador no reconocido\n");
            return 0;  // Si el operador no es válido, retornamos 0.
    }

    return resultado;
}

int main() {
    int resultado;
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    int slen = sizeof(server_addr);
    int clen = sizeof(client_addr);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bind(server_socket, (struct sockaddr *) &server_addr, slen) == -1) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 1) == -1) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("servidor: esperando conexión del cliente...\n");

    while(1) {
        //acepto link
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
        if (client_socket == -1) {
            perror("Error en accept");
            exit(EXIT_FAILURE);
        }
        
        int pid = fork();

        if(pid == 0){
            printf("servidor recibe nuevo cliente \n");
            char expresion[256];
            while(1){
                ssize_t tamano_mensaje = recv(client_socket,expresion,256,0);
                if(tamano_mensaje == 0){
                    exit(0);
                }
                resultado = calcular(expresion);
                printf("%d \n", resultado);
                send(client_socket, &resultado, sizeof(int), 0);   
            }  
        }
    }
    close(client_socket);
    unlink(server_addr.sun_path);
    exit(0);
}


