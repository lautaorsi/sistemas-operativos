#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n;
int numero_maldito;
pid_t PIDs[10];

int generate_random_number(){
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void handler15(){
    // printf("handler \n");
    // int numero_random = generate_random_number();
    int numero_random = 1;
    printf("Numero random: %d \n", numero_random);
    if (numero_maldito == numero_random){
        printf("Hasta la proxima \n");
        exit(0);
    } 
}

void subrutinaHijo(){
    signal(SIGTERM, handler15);

    // printf("subrutina \n");
    while(1){};
}

void handler_sigchild(){
    pid_t pid = wait(NULL);
    printf("recibi sigchild: %d \n", pid);

    printf("valor de n: %d \n", n);
    for(int i = 0; i < 10; i++){ 
        if(PIDs[i] == pid){ 
            PIDs[i] = 0;
        }
        
    };
}


int main(int argc, char const *argv[]){
    
    int n = atoi(argv[1]);
	int rondas = atoi(argv[2]);
	numero_maldito = atoi(argv[3]);

    int r = generate_random_number();
    printf("random number: %d \n", r);

    
    if (n == 0) return 0;
    signal(SIGCHLD, handler_sigchild);
    pid_t pid_padre = getpid();
    printf("pid padre: %d \n", pid_padre);

    for(int i = 0; i < n; i++){
        PIDs[i] = fork();

        if(PIDs[i] == 0) {
            printf("asd \n");
            subrutinaHijo();
        };
    }
    sleep(3);

    while(rondas != 0){
        rondas --;
        for(int i = 0; i < n; i++){
            if(PIDs[i] != 0){
                kill(PIDs[i],SIGTERM);
                sleep(1);
            }
            
            
        }   
    }

    for(int i = 0; i < n;i++){
        if(PIDs[i] != 0){    
            printf("proceso %d sobrevivió \n", PIDs[i]);
            kill(PIDs[i], SIGKILL);
        }
    };

    
    
    exit(0);
}
