# Seguridad

Dado el siguiente código:

    #define BUFFER_SIZE 12

    unsigned int f(){
        unsigned int len;
        char buffer[BUFFER_SIZE];
        printf("Ingrese su nombre:");
        gets(buffer);
        len = strln(buffer);
        return len;
    }

### a) Que problemas de seguridad tiene?

Podriamos generar un buffer overflow y pisar el instruction pointer de retorno que vive en el stack

### b) Dibujar el stack

Al ser 32 bits el ancho del stack es de 4 bytes

``
--- direcciones bajas ---

+++ 
buffer[0:3]
buffer[4:7]                 STACK FUNCION LLAMADA
buffer[8:11]
len
saved ebp
+++

+++
Ret addr
parametros                  STACK FUNCION LLAMADORA
...
+++
--- direcciones altas ---

``

### c) Como solucionarlo
Cambiamos gets por `fgets` para no leer mas de los bytes del buffer => `fgets(buffer, BUFFER_SIZE, STDIN_FILENO);`

### d) Explicar Stack Canary y si soluciona los problemas

El compilador va a agregar fragmentos de código alrededor de las llamadas a las funciones que inserta "canarios" y verifica que este siga estando en la misma posición y con el valor no modificado. <br>
Podemos entonces solucionar el problema insertando un canario justo arriba del save ebp, para asegurarnos que no pise nada mas allá del buffer y len.


# Drivers

Variables globales:
- int procesos_activos = 0;
- int luces[5] = [1,2,3,4,5];
- sem_t semaforo_procesos_activos = semaforo(1);


void driver_init(){

}

int driver_open(){

    semaforo_procesos_activos.wait();

    if (procesos_activos >= 5){
        return -1;
    }
    
    procesos_activos++;
    numero_id = random(luces);
    luces.sacar(numero_id);

    semaforo_procesos_activos.signal();

    return numero_id;

}

void driver_close(int numero_id){
    semaforo_procesos_activos.wait();
    procesos_activos --;
    luces.agregar(numero_id);
    semaforo_procesos_activos.signal();
}

int driver_read(){
    return IN(RTSA_SENIAL);
}

int driver_write(int numero_id, int encender){  //Asumo que encender == 1 si quiere prender y 0 si quiere apagar

    OUT(RTSA_NUM_LED, numero_id, sizeof(int));
    
    if(encender == 1){
        OUT(RTSA_STATUS_LED, RTSA_LED_ON);    
    }
    else{
        OUT(RTSA_STATUS_LED, RTSA_LED_OFF)
    }

}

int driver_remove(){

}



# Filesystem

Ej 11 de la guía

Estrategia:

- Conseguir inodo del path dir
- Recorrer bloques de datos del inodo
- Mirar dir entries
    - Comparo nombre y tamaño
        - Si cumple lo imprimo (o agrego al array)
    - Si es dir (y no son "." o "..") hago recursión

    find_file_less_size(char* dir, int min_bytes, char* arch_nombre){
        Ext2FSInode* inode = inode_for_path(dir);
        ffls_recursive(inodo, min_bytes, name);
    }

    ffls_recursive(inodo, minbytes, name){

        char* buf = (char*) malloc(2 * BLOCK_SIZE);
        char* buf2 = buf + BLOCK_SIZE;
        int blocknum = 0;
        int recorrido = 0;
        int directory_entry_pointer = 0;

        int blockad = get_block_address(block_number, inodo);
        int blockad2 = get_block_address(block_number + 1, inodo);                      //Calculo numero de bloques

        read_block(buf,blockad);
        read_block(buf2, blockad2);                                                     //Leo bloques

        while(recorrido < inodo->size){                                      
            //Resolver ajuste de buffer
            Ext2FSDirEntry dentry = (Ext2FSDirEntry*) (buf + dentrypointer);    
            if(!strncmp(dentry->name, name, dentry->sizename)){                           //ESTO ES IMPORTANTE PARA EVITAR SUFIJOS/PREFIJOS
                //Ver size en inodo, si son iguales lo printeo
            }
            if(dentry->fileType == 'd' && dentry->name >= 2){
                Ext2FSInode* inodoD = load_inode(dentry->inode);
                ffls_rec(inodoD, minbytes, name);
            }
            //actualizar punteros contadores
        }

    }