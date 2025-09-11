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


void procesoHijoInicial(){
	int valor_inicial;
	read(pipes[n][READ], &valor_inicial, sizeof(int));
	printf("valor inicial: %d \n", valor_inicial);
	randomNum = generate_random_number();
	while(randomNum < valor_inicial){
		randomNum = generate_random_number();
	}
	printf("numero random: %d \n", randomNum);
	write(pipes[proceso_i][WRITE], &randomNum, sizeof(int));
	procesoHijo();
}

void procesoHijo(){
	printf("AAA soy hijo proceso %d \n", proceso_i);
	//cierro pipes salvo 2 que me interesan
	for(int i = 0; i < n; i++){
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
	printf("proceso %d cerro pipes \n", proceso_i);
	int numero;
	while(1){
		//espero a escritura del proceso anterior
		if(proceso_i == 0){
			printf("proceso %d espera escritura \n", proceso_i);
			read(pipes[n][READ],&numero, sizeof(numero));	
		}
		else{
			printf("proceso %d espera escritura \n", proceso_i);
			read(pipes[proceso_i-1][READ], &numero, sizeof(int));
		}
		printf("recibo numero: %d \n", numero);
		
		//si soy proceso iniciador me fijo si el numero secreto es menor al actual
		if(proceso_i == start && randomNum < numero){
			printf("%d no es menor a numero actual \n", numero);
			//si es menor, mando el resultado a proceso padre
			write(pipes[n+2][WRITE], &numero, sizeof(int));
			exit(1);
		}
		//incremento y escribo si no soy proceso iniciador y/o no es mayor a numero secreto
		numero += 1;
		printf("envio numero %d \n", numero);
		write(pipes[proceso_i][WRITE], &numero, sizeof(int));
	}
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
	for(int i = 0; i <= n +1; i++){
		pipe(pipes[i]);
	}
	
	for(int i = 0; i < n; i++){
		int pid = 0;
		int proceso_i = i;
		pid = fork();
		pids[i] = pid;
		
		if(pid == 0 && i != start){
			printf("soy hijo proceso: %d \n", proceso_i);
			procesoHijo();
		}
		if(pid == 0 && i == start){
			printf("soy hijo proceso inicial: %d \n", i);
			procesoHijoInicial();
		}
	}

	write(pipes[n][WRITE], &start, sizeof(int));

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
