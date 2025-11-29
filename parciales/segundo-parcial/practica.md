## Ejercicio 2

    estructuras:

    struct registro_t{
        int es_final;
        int coordenada;
        int tiempo;
    };

    struct coloreo{
        int coordenada;
        int 
    }

    variables globales:


    void driver_init(){
        irq_handler(IRQ_TOUCH, handler_gesto);
    }

    driver_read(void* data){
        
        semaforo_read.wait();

        while( OUT(TCH_STATUS) == TCH_PRESS ){                      //Mientras esté apretando

            registro_t registro.coordenada = OUT(TCH_COORD);        //Extraigo coordenadas
            
            registro.es_final = 0;

            registro.tiempo = OUT(TCH_TIME);                        //Extraigo duración de gesto

            copy_to_user(data, registro);                           //Envio el dato
        }
            
        registro.tiempo = OUT(TCH_TIME);                        //Extraigo duración de gesto final

        registro.es_final = 1;

        copy_to_user(data, registro);                           //Hago ultimo envio de datos
    }

    driver_write(void* data){
        
    }

    driver_remove(){}

    void handler_gesto(){
        semaforo_read.signal();         //Cuando esta disponible la lectura
    }



## Ejercicio 1


    void listar(){
        superbloque->
    }