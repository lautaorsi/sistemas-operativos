#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

enum {READ, WRITE};
int (*pipes)[2];
int randomNum, n, proceso_i, start;




int generate_random_number(){
	return (rand() % 50);
}

void procesoHijo(){
	//cierro pipes salvo 2 que me interesan
	for(int i = 0; i < n; i++){
		if(proceso_i == start){
			if(i != proceso_i && i != n - 1){
				close(pipes[i][READ]);
				close(pipes[i][WRITE]);
			}
			if(i == proceso_i){
				close(pipes[i][READ]);
			}
			if(i == n - 1){
				close(pipes[i][WRITE]);
			}
		}
		else{
			if(i != proceso_i && i != proceso_i - 1){
				close(pipes[i][READ]);
				close(pipes[i][WRITE]);
			}
			if(i == proceso_i){
				close(pipes[i][READ]);
			}
			if(i == proceso_i - 1){
				close(pipes[i][WRITE]);
			}
		}
	}
	int numero;
	while(1){
		//espero a escritura del proceso anterior
		if(proceso_i == 0){
			if(read(pipes[n-1][READ],&numero, sizeof(int)) == -1){
				break;
			}
		}
		else{
			if(read(pipes[proceso_i-1][READ], &numero, sizeof(int)) == -1){
				break;
			}
		}
		//si soy proceso iniciador me fijo si el numero secreto es menor al actual
		if(proceso_i == start && randomNum <= numero){
			
			//si es menor, mando el resultado a proceso padre
			write(pipes[n+1][WRITE], &numero, sizeof(int));
			break;
		}
		//incremento y escribo si no soy proceso iniciador y/o no es mayor a numero secreto
		numero += 1;
		write(pipes[proceso_i][WRITE], &numero, sizeof(int));
	}
}

void procesoHijoInicial(){
	int valor_inicial;
	read(pipes[n][READ], &valor_inicial, sizeof(int));
	randomNum = generate_random_number();
	while(randomNum < valor_inicial){
		randomNum = generate_random_number();
	}
	printf("numero random: %d de hijo %d \n",randomNum, proceso_i);
	write(pipes[proceso_i][WRITE], &randomNum, sizeof(int));
	procesoHijo();
}


int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
	//necesito n pipes para la comunicacion entre subprocesos y 2 pipes para comunicacion bidireccional con el padre
	int dato;
	pipes = malloc(sizeof(*pipes) * (n + 2));
	int pids[n];

	//creo pipes
	for(int i = 0; i <= n + 1; i++){
		pipe(pipes[i]);
	}
	
	for(int i = 0; i < n; i++){
		int pid = 0;
		proceso_i = i;
		pid = fork();
		pids[i] = pid;
		
		if(pid == 0 && i != start){
			procesoHijo();
		}
		if(pid == 0 && i == start){
			procesoHijoInicial();
		}
	}

	write(pipes[n][WRITE], &buffer, sizeof(int));

	//cierro pipes salvo ultimos 2
	for(int i = 0; i < n; i++){
		close(pipes[i][READ]);
		close(pipes[i][WRITE]);
	}
	
	
	read(pipes[n+1][READ], &dato, sizeof(int));
	
	for(int i = 0; i < n; i++){
		kill(9, pids[i]);
	}

	printf("numero: %d \n",dato);
    /* COMPLETAR */
    
}
