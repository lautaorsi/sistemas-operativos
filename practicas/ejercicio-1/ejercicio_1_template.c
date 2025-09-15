#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() write() read()
#include "dado.h"    // tirar_dado()

// Constantes 0 / 1 para READ / WRITE
enum { READ, WRITE };
// Constantes 0 / 1 para LESTER / ELIZA
enum { LESTER, ELIZA };

// Variables globales
int pipes_simpson[2][2];

void lester() {
  // Tiro el dado
  int dado_lester = tirar_dado();
  // Le informo el resultado a Humberto
  write(pipes_simpson[LESTER][WRITE],
        &dado_lester,
        sizeof(dado_lester));
  exit(EXIT_SUCCESS);
}

void eliza() {
  // Tiro el dado
  int dado_eliza = tirar_dado();
  // Le informo el resultado a Humberto
  write(pipes_simpson[ELIZA][WRITE],
        &dado_eliza,
        sizeof(dado_eliza));
  exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
  // Creo los pipes
  for(int i = 0; i<2; i++){
    pipe(pipes_simpson[i]);
  }
  // pipes_simpson[LESTER][READ] 
  // será el fd del extremo 
  // de lectura de LESTER

  // Creo a Lester
  pid_t pid_lester = fork();
  if(pid_lester==0){ // Soy el hijo
    lester();
  }

  // Creo a Eliza
  pid_t pid_eliza = fork();
  if(pid_eliza==0){ // Soy la hija
    eliza();
  }

  // Recibo el dado de Lester
  int dado_lester_resultado;
  read(pipes_simpson[LESTER][READ],
       &dado_lester_resultado,
      sizeof(dado_lester_resultado));

  // Recibo el dado de Eliza
  int dado_eliza_resultado;
  read(pipes_simpson[ELIZA][READ],
       &dado_eliza_resultado,
      sizeof(dado_eliza_resultado));

  // Anuncio al ganador
  if(dado_eliza_resultado>dado_lester_resultado){
    printf("Gano Eliza porque sacó %d", dado_eliza_resultado);
  }else{
    printf("Empate o gano Lester");
  }

  return 0;
}
