## Ejercicio 1
Es una pieza de software, aplicación de usuario, gestor de interrupciones y conoce el dispositivo que controla pero no el SO (a,d,e,f)

## Ejercicio 2 

Constantes:
CHRONO_RESET

    int driver_write(){
        OUT(CHRONO_CONTROL, CHRONO_RESET);
        return 0;
    }

    int driver_read(){
        return IN(CHRONO_CURRENT_TIME);
    }

## Ejercicio 3

    int driver_read(){
        while(true){
            int status = IN(BTN_STATUS);
            if(status[0]){
                OUT(BTN_STATUS, 0);
                return BTN_PRESSED;
            }
        }
    }

## Ejercicio 4

vars globales
sem_t semaforo

    void driver_init(){
        irq_register(7, handler);
        sem(semaforo, 0);
    }

    int driver_read(){
        semaforo.wait();
        int status = IN(BTN_STATUS);
        if(status[0]){
            OUT(BTN_STATUS, 0);
            return BTN_PRESSED;
        }
    }

    void handler(){
        semaforo.signal();
    }


## Ejercicio 7

Registros

- Escritura
    - DOR_IO
    - ARM
    - SEEK_SECTOR

- Lectura
    - DOR_STATUS
    - ARM_STATUS
    - DATA_READY

### a)
vars globales 
sem_t semaforo_brazo;

    int driver_wirte(int sector, void *data){
        wait(semaforo_brazo);
        void* datos = malloc(sizeof(data));
        copy_from_user(datos,data, sizeof(data));

        OUT(DOR_IO, 1); //Enciendo y espero 50ms
        sleep(50);
        
        int pista = sector / cantidad_sectores_por_pista();

        int indice_pista = sector % cantidad_sectores_por_pista();  //Calculo indice dentro de pista
        
        OUT(ARM, pista);   //hago q el brazo vaya a pista
        
        while(IN(ARM_STATUS)){} //Busy waiting hasta q el brazo este en posición

        OUT(SEEK_SECTOR, indice_pista); //voy a sector en pista

        escribir_datos(datos);  //Escribo datos

        while(!IN(DATA_READY)){}    //Espero hasta que termine de escribirse

        OUT(DOR_IO, 0);             //Apago brazo

        sleep(200);                 //Espero 200ms para que no se pueda hacer otra operacion en el medio

        signal(semaforo_brazo);
    }
    
### b)

vars globales
sem_t semaforo_ARM_STATUS, semaforo_DATA_READY, semaforo_TIMER, semaforo_brazo;

    int driver_init(){
        irq_handler(6, handler_disco);
        irq_handler(7, handler_timer);  //Vinculo handlers

        semaforo_ARM_STATUS = semaforo(0);
        semaforo_DATA_READY = semaforo(0);
        semaforo_timer = semaforo(0);
        semaforo_brazo = semaforo(1);       //Armo semaforos
    }

    int driver_write(int sector, void *data){
        wait(semaforo_brazo);                   //Impedimos que dos procesos accedan al driver en simultaneo
        void* datos = malloc(sizeof(data));     //Armamos buffer para dato
        copy_from_user(datos,data,sizeof(data));
        
        OUT(DOR_IO, 1); //Enciendo y espero 50ms
        wait(semaforo_TIMER);   
        wait(semaforo_TIMER);   //Lo hacemos dos veces porque la interrupción probablemente no llegue alineada

        int pista = sector / cantidad_sectores_por_pista();

        int indice_pista = sector % cantidad_sectores_por_pista();  //Calculo indice dentro de pista

        OUT(ARM, pista);                                            //hago q el brazo vaya a pista

        wait(semaforo_ARM_STATUS);

        OUT(SEEK_SECTOR, indice_pista);                             //voy a sector en pista        

        escribir_datos(datos);                                      //Escribo datos

        wait(semaforo_DATA_READY);                                  //Espero a que se haya terminado de escribir

        OUT(DOR_IO, 0);                                             //Apago brazo

        for(int i = 0; i < 4; i++){
            wait(semaforo_TIMER);
        }
        signal(semaforo_brazo);
    }

    void handler_disco(){
        
        
        if(IN(DATA_READY)){                                         //Si la señal llegó por DATA_READY
            signal(semaforo_DATA_READY);
        }


        if(IN(ARM_STATUS)){                                         //Si la señal llegó por ARM_STATUS
            signal(semaforo_ARM_STATUS);
        }
    }

    void handler_timer(){
        signal(semaforo_TIMER);
    }


## Ejercicio 8

    variables globales:
    sem_t semaforo_escritura, semaforo_escribiendo;

    int driver_init(){
        irq_handler(HP_FINISHED_INT, handler_terminado);
        semaforo_escritura = sem(1);
        semaforo_escribiendo = sem(0);
    }

    int driver_remote(){
        irq_free(HP_FINISHED_INT);
        semaforo_escritura.destroy();
        semaforo_escribiendo.destroy();
    }

    int driver_write(void *data, int size){
        semaforo_escritura.wait();              //Bloqueo race conditions

        char* buffer = (char *) malloc(size);   //Armo buffer
        copy_from_user(buffer,data,size);       //Copio 
        for(int i = 0; i < 5; i++){
            OUT(LOC_TEXT_POINTER,buffer);       //Pruebo mandar los datos 5 veces
            OUT(LOC_TEXT_SIZE, size);
            OUT(LOC_CTRL, START);
            
            if(IN(LOC_CTRL) == PRINTING){               // si fue valida no le mando mas intentos, cc tengo que seguir intentando
                break;
            }
        }

        semaforo_escribiendo.wait();

        semaforo_escritura.signal();            //Habilito a otros procesos a usar
    }

    void handler_terminado(){
        semaforo_escribiendo.signal();
    }


