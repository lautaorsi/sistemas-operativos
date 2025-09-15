#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

enum {READ, WRITE};


static int run(char ***progs, size_t count)
{	
	int r, status;
	pid_t pid;
	int (*pipes)[2] = malloc(sizeof(*pipes) * (count-1));
	int proceso_i;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);
	//TODO: Pensar cuantos procesos necesito
	//RTA: 1 proceso por comando? 
	
	//TODO: Pensar cuantos pipes necesito.
	//RTA: #procesos - 1 pipes? ej 3 procesos, A comunica a B, B a C => 2 pipes
	
	//creo pipes
	for(int i = 0; i < count - 1; i++){
		printf("creo pipe: %d \n", proceso_i);
		pipe(pipes[i]);
		printf("pipe creado: %d \n", proceso_i);
	}
	//creo procesos
	for(int i = 0; i < count; i++){
		proceso_i = i;
		children[i] = fork();
		pid = children[i];
		if(pid == 0)break;
		else printf("creado hijo: %d, pid: %d \n", proceso_i, pid);
	}

	if(pid != 0){
		printf("soy padre");
		//cierro todos los pipes
		for(int i = 0; i < count - 1 ; i++){
			close(pipes[i][READ]);
			close(pipes[i][WRITE]);
		}
	}


	if(pid == 0){  //si soy hijo
		printf("soy hijo: %d \n", proceso_i);

		
		char* input;

		//Primer hijo
		if(proceso_i == 0){

			dup2(pipes[proceso_i][WRITE], STD_OUTPUT);
			//Cierra todo salvo su write
			close(pipes[proceso_i][READ]);
			for(int i = 1; i < count -2; i++){
				close(pipes[i][READ]);
				close(pipes[i][WRITE]);
			}

			//ejecuto comando
			execvp(progs[proceso_i][0], progs[proceso_i]);
		}
		//Ultimo hijo
		if(proceso_i == count -1){
			dup2(pipes[proceso_i - 1][READ], STD_INPUT);

			//Cierro todo salvo el read del proceso anterior
			close(pipes[proceso_i -1][WRITE]);
			for(int i = 0; i < count - 2; i++){
				close(pipes[i][READ]);
				close(pipes[i][WRITE]);
			}
		}
		//cualquier otro hijo 
		if(proceso_i != 0 && proceso_i != count-1){
			dup2(pipes[proceso_i][WRITE], STD_OUTPUT);
			dup2(pipes[proceso_i - 1][READ], STD_INPUT);
			for(int i = 0; i < count - 1; i++){
				if(i != proceso_i - 1){
					close(pipes[i][WRITE]);
				}
				else{
					if(i != proceso_i){
						close(pipes[i][WRITE]);
					}
					close(pipes[i][READ]);
				}
			}
		}

		//ejecuto comando
		execvp(progs[proceso_i][0], progs[proceso_i]);
	}

	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

	//Espero a los hijos y verifico el estado que terminaron
	if(pid != 0){
		for (int i = 0; i < count; i++) {
			waitpid(children[i], &status, 0);
			if (!WIFEXITED(status)) {
				fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
				    (int)children[i], WIFSIGNALED(status));
				perror("");
				return -1;
			}
		}
	}
	free(pipes);
	r = 0;
	free(children);
	return r;
}


int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n"); 
		printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
		return 0;
	}
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
