#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char MAX_SIZE = 127;
unsigned char buffer[128];
char *format = "%s %d %s\n";

char* algo_asi_si(char *cadena){
    scanf(" %127s", buffer);
    printf(format, buffer, MAX_SIZE, cadena);
    return cadena;
}

char* algo_asi_no(char *cadena){
    if(strlen(cadena) > MAX_SIZE) exit(55);
    sprintf(buffer, format, "echo" , atoi(cadena), "asi si?\n");
    system(buffer);
    return cadena;
}

int existe_archivo(char* foo){
    return 0;
}

void registrar_ganador(char *nombre_ganador, char *frase) {
    char directorio_ganadores[] = ".";
    char archivo_ganador[256];
    snprintf(archivo_ganador, sizeof(archivo_ganador), "%s/%s",
    directorio_ganadores, nombre_ganador);

    if(!existe_archivo(archivo_ganador)) {
        char command[512];
        snprintf(command, sizeof command, "echo ' %s' > %s",
        frase, archivo_ganador);
        system(command);
    }
}

int main(int argc, char **argv){

    //Por favor tener mucho cuidado a la hora de probar este codigo,
    //si le ponen permisos de root y hacen cagadas en su sistema
    //no me hago responsable. Que la fuerza los acompañe

    //Activarlo si corres el programa en linux, windows no reconoce este comando
    //setuid(0);
    
    //Ejercicio 17
    //printf(algo_asi_no(algo_asi_si(argv[1])));
    

    //Ejercicio 18
    //registrar_ganador(argv[1], argv[2]);
}