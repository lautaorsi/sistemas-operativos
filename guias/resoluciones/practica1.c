#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/sched.h>

int pid_padre, pid_hijo, contador, interaccion_finalizada;

int ej5(){
    
    int pid_hijo = fork();              //Forkeamos el proceso, fork() retorna el PID del hijo para la ejecucion del padre y cero para el hijo   
    if(!pid_hijo){
        printf("Soy Homero! \n");       //Unicamente homero ejecuta esta parte, a esta altura estan vivos Abraham y Homero

        int pid_Bart = fork();              //Creamos a Bart
        if(!pid_Bart){                  //Ejecutado x Bart
            printf("Soy Bart! \n");
            exit(1);                    //Muere Bart
        }

        int pid_Lisa = fork();              //Creamos a Lisa
        if(!pid_Lisa){                  //Ejecutado x Lisa
            printf("Soy Lisa! \n");
            exit(1);                    //Muere Lisa
        }

        int pid_Maggie = fork();              //Creamos a Maggie
        if(!pid_Maggie){                  //Ejecutado x Maggie
            printf("Soy Maggie! \n");
            exit(1);                    //Muere Maggie
        }

        waitpid(pid_Bart, NULL, 0);
        waitpid(pid_Lisa, NULL, 0);
        waitpid(pid_Maggie, NULL, 0);     //Esperamos a que mueran los 3 

        exit(1);                        //Muere Homero
    }; 
    printf("Soy Abraham! \n");          //Ejecutado x Abraham
    exit(1);                            //Muere Abraham 
}                                       

void ej6(const char *arg){
    pid_t pid = fork();
    execl("/bin/sh", "sh", "-c", arg, (char*) NULL);
}

void pong(){
    printf("Pong \n ");
}

void subrutinaHijo(){
    signal(SIGTERM,pong);
    pause();
}

void ej9(){
    int keep_going;
    pid_t pidHijo = fork();

    if(pidHijo == 0) subrutinaHijo();
    
    while(1){
        for(int i = 0; i < 3; i++){
            printf("Ping \n ");
            kill(SIGTERM, pidHijo);
            pause();
        }
        printf("Want to keep going? [1/0] ");
        scanf("%d", &keep_going);
        if(!keep_going) {
            kill( SIGKILL, pidHijo);
            exit(1);
        };
    }

}

void ej10(){
    int clone_pid = clone();
    if(clone_pid == 0){
        printf("soy Julieta\n");
        int clone2_pid = clone();
        if(clone2_pid == 0){
            printf("Soy Jennifer");
        }
        exit(0);
    }
    printf("soy Juan\n");
    wait(NULL);
    clone_pid = clone();
    if(clone_pid == 0){
        printf("soy jorge\n");
    }
    exit(0);
}

void handlePong(){
    printf("Pong \n");
    kill(SIGTERM, pid_padre);
    pause();
}

void handlePing(){
    while(contador < 3){
        contador++;
        printf("Ping \n");
        kill(SIGINT, pid_hijo);
        pause();
    }    
}

int ej9_p2(){
    printf("asd \n");
    pid_padre = getpid();
    pid_hijo = fork();
    signal(SIGINT, handlePong);
    signal(SIGTERM, handlePing);
    if (pid_hijo == 0){
        pid_hijo = getpid();
        
    }

    int i = 0;
    while(i >= 0){
        contador = 1;
        printf("Ping \n");
        kill(SIGINT, pid_hijo);
        pause();
        i--;
    }

    kill(SIGKILL, pid_hijo);
    exit(0);

}

int main(){
    ej9_p2();
    return 0;
}


