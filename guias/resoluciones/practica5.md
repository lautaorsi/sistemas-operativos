### Ejercicio 1
Es una pieza de software, aplicación de usuario, gestor de interrupciones y conoce el dispositivo que controla pero no el SO (a,d,e,f)

### Ejercicio 2 

Constantes:
CHRONO_RESET

    int driver_write(){
        OUT(CHRONO_CONTROL, CHRONO_RESET);
        return 0;
    }

    int driver_read(){
        return IN(CHRONO_CURRENT_TIME);
    }

### Ejercicio 3

    int driver_read(){
        while(true){
            int status = IN(BTN_STATUS);
            if(status[0]){
                OUT(BTN_STATUS, 0);
                return BTN_PRESSED;
            }
        }
    }

### Ejercicio 4

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


### Ejercicio 7

Registros

- Escritura
    - DOR_IO
    - ARM
    - SEEK_SECTOR

- Lectura
    - DOR_STATUS
    - ARM_STATUS
    - DATA_READY

**a)**

    int driver_wirte(int sector, void *data){
        void* datos;
        copy_from_user(datos,data, sizeof(data));
        
        sleep(200);

        OUT(DOR_IO, 1); //Enciendo y espero 50ms
        sleep(50);
        
        //Ignoro la parte de pista y voy directo a sector
        
        OUT(ARM, sector);   //hago q el brazo vaya a sector

        while(IN(ARM_STATUS)){} //Busy waiting hasta q el brazo este en posición

        escribir_datos(datos);

        while(!IN(DATA_READY)){}

        OUT(DOR_IO, 0);
    }
    
**b)**

vars globales
sem_t semaforo_escritura;

    int driver_init(){
        irq_handler(6, handler_disco);
        irq_handler(7, handler_timer);
    }

    int driver_write(int sector, void *data){
        void* datos;
        copy_from_user(datos,data,sizeof(data));

        
        
    }