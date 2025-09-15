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
	char operacion[100];
	while(1){
		scanf("%s", operacion);
		
			
		if(strcmp(operacion,"exit") == 0){
			exit(0);
		}
			
		if(send(server_socket, &operacion, strlen(operacion), 0) == -1){
			perror("Error");
			exit(EXIT_FAILURE);
		};
	}


	exit(0);
}
