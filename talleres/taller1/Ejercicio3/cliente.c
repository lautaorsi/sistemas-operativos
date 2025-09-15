#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
	
    int server_socket;
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    
	if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
	
	printf("conexion validada \n");
	char operacion[256];
	while(1){
		scanf("%s", operacion);
		//si es exit el msj salgo
		if(strcmp(operacion,"exit") == 0){
			break;
		}
		//mando mensaje
		if(send(server_socket,&operacion,256,0) == -1){
			perror("Error");
			exit(EXIT_FAILURE);
		};
		//espero procesamiento del sv
		int resultado;
		if(recv(server_socket,&resultado,sizeof(int),0) == -1){
			perror("Error");
			exit(EXIT_FAILURE);
		}
		//printeo resultado
		printf("resultado: %d \n", resultado);
		
	}


	exit(0);
}
