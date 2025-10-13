## Drivers 

### Ejercicio 1: Robot y cajas

Nuestros registros:
- _LOC_TARGET_                            **<- Write**
- _LOC_CTRL_ $\in$ {START, JOYA, BAJON}   **<- Read, Write** 
- _LOC_STATUS_ $\in$ {BUSY, READY}        **<- Read**

Tomemos el siguiente codigo, cuales acceden al driver? **(open y write)**

    int main (int argc, char *argv[]) {
        int robot = open("/dev/chinbot", "w");
        int codigo;
        int resultado;
        while (1) {
            printf("Ingrese el código de la caja\n");
            scanf ("%d", &codigo);
            resultado = write(robot, codigo);
            if (resultado == 1) {
                printf("Su orden ha llegado\n");
            }else {
                printf("No podemos encontrar su caja %d\n", codigo);
            }
        }
    }

Codear int driver_write(*data) _(invocada al solicitarse un write)_

    int driver_write(*data){                    
        while(IN(LOC_STATUS) == READY)              //Mientras el registro STAUTS esta en READ no hace nada, salimos cuando nos mandan START
        int cod;
        copy_from_user($cod, data, sizeof(int));    //Ojo con el puntero *data, es pasado x el usuario y estamos a lvl kernel
        mutex.wait()                                //SECCION CRITICA
        OUT(LOC_TARGET, cod);
        OUT(LOC_TARGET, *data);                     //Escribimos en registro TARGET el dato 
        OUT(LOC_CTRL, START);                       //Escribimos en CTRL START, para que comience la ejec
        while(in(LOC_CTRL) == START){               //mientras el registro CTRL este en start 
            res = IN(LOC_CTRL)                      //guardo en res lo del registro CTRL
            if(res == JOYA){                        //si el CTRL nos indica joya salimos con codigo 1
                return 1
            }
            else{                                   //caso contrario salimos con codigo 0
                return 0
            }
        }
        mutex.signal()                              //FIN SECCION CRITICA
    }

Modificar el codigo anterior para utilizar interrupciones
- **int request_irq(int irq, void *handler)**: Permite asociar el procedimiento handler a la interrupción IRQ. Devuelve IRQ_ERROR si ya está asociada a otro handler.
- **int free_irq(int irq)**: Libera la interrupción IRQ del procedimiento asociado.

        //La interrupcion la manda el robot

        vars globales:
        semaforo_t mutex;       //compartida
        semaforo_t semaforo;    //compartida
        bool esperando;         //NO compartida

        int driver_init(){
            reqiure_irq(CHINBOT_INT, handler);
            mutex = mutex(1);
            semaforo = semaforo(0);
            esperando = 0;
        }


        int handler(){
            if(esperando){
                semaforo.wait();
                esperando = 0;
            }
        }


        int driver_write(*data){                    
            int cod;
            copy_from_user($cod, data, sizeof(int));    //Ojo con el puntero *data, es pasado x el usuario y estamos a lvl kernel
            mutex.wait();                                //SECCION CRITICA
            esperando = 1;
            OUT(LOC_TARGET, cod);
            OUT(LOC_CTRL, START);                       //Escribimos en CTRL START, para que comience la ejec
            res = IN(LOC_CTRL)
            mutex.signal()                              //FIN SECCION CRITICA
            if(resultado == JOYA){
                return 1
            }
            else{
                return 0
            }
        }