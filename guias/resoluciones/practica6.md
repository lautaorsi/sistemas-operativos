# Sistemas de archivos


## Ejercicio 2

Tamaños de referencia: 
- 1024 Bytes = 2 ^10 Bytes
- 1 KB = 2 ^10 Bytes
- 8 KB = 2 ^13 Bytes
- 1 GB = 2 ^30 Bytes
- 128 GB = 2 ^37 Bytes

### a)
Sabemos que la tabla indexa por numero de bloque la totalidad del disco y por lo tanto tendriamos <br>
```128 GB / 8 KB = 2 ^24``` elementos, con entradas de 24 Bytes, resultando en un total de <br>
```2^24 B * 24 B =  (2^27 * 3) B =  384 MB``` de memoria ocupada por la tabla

### b)
Sabemos que tenemos una capacidad de 128 GB - 384 MB (por el espacio ocupado por la tabla)
resultando en  
``127.616 GB disponibles = 130,678.784 MB => 13.067 archivos`` <br> 
**OJO: hay que hacer las cuentas en BLOQUES, no bytes ya que los bloques se usan en su totalidad, incluso si sobra un poco luego del EOF, CORREGIR EJERCICIO**

### c)
Tenemos 8 indices, entonces el rango del tamaño del archivo es de 64 KB

## Ejercicio 3

Tenemos 8 entradas por inodo distribuidas en:
- 5 entradas directas
- 2 entradas indirectas simples
- 1 entrada indirecta doble

### a)
Para calcular el espacio maximo tomemos en cuenta lo siguiente:
- 5 entradas directas que apuntan a bloques de datos &#8594; 10 bloques
- 2 entradas indirectas simples que apuntan a bloques de datos de 4KB con LBAs de 8B &#8594; 2 * (4 * 1024 / 8) = 1024 bloques
- 1 entrada indirecta doble que apunta a 4kb de entradas indirectas simples con LBAs de 8B &#8594; 512^2 bloques

Por lo tanto tenemos una capacidad maxima aproximada de **2^28 + 10 bloques de 4KB**

## Ejercicio 4

Bloques de 4KB, supongamos que los inodos tienen 5 entradas directas, 1 indirecta simple y 1 indirecta doble

### a)
Deberiamos hacer 5 lecturas de los bloques directos, 1 lectura del bloque de indireccion simple y 5 lecturas de las direcciones para los bloques faltantes

### b)
Nuevamente hacemos 5 lecturas de los bloques directos, 1 lectura del bloque de indireccion simple y las 16 lecturas de las direcciones restantes. (Sabemos que los 16 van a estar en la indireccion simple ya que si asumimos LBAs de 8B y el bloque mide 4KB entran 2^12 / 2^3 = 2^9 LBAs)

### Agregado
Imagino que la gracia estaba en ver que si tenemos una indireccion doble/triple hay que sumar las lecturas a bloques que contiene las LBAs, no se si el item b y a son iguales por mi interpretación de estructura de inodos o que, pero cuidado

## Ejercicio 7

### a)
Es solo para inodos 

### b)
FAT, si lo que queremos es tener todo en un sector necesitamos la tabla, ya que para los inodos vamos a tener todo mas distribuido

### c)
FAT, si usamos inodos vamos a estar limitados por la estructura del mismo

### d)
Inodos, en FAT vamos a tener que estar cargando la tabla 

## Ejercicio 10

### a)
    struct struct_entrada_directorio{
        char* nombre;
        int primer_bloque_address;
        int tamaño;
        struct_entrada_directorio* subdirectorios;
    }

### b)
    void cargar_archivo(directorios[]){
        struct_entrada_directorio actual = root_table();
        
        for(int i = 0; i < directorios.size; i++){                                  //Itero por lista de entrada
            
            struct_entrada_directorio* subdirectorios = actual.subdirectorios;

            for(int j = 0; j < actual.tamaño; j++){                                 //Itero por subdirectorios en directorio actual
                 
                if( strcmp(directorios[i],subdirectorios[j].nombre) ){              //Si tienen el mismo nombre
                    
                    if(i == directorios.size - 1){                                  //Y es el archivo final, armo lista de bloques, cargo y retorno
                        int bloque = actual.primer_bloque_address;
                        int bloques[];                                              //Me permito cometer un crimen y armo un array modificable para no morir en tecnicismos 
                        while(bloque != EOF){
                            bloques.append(bloque);
                            bloque = FAT_entry(bloque);                             //Guardo en lista los numeros de bloques hasta llegar al EOF
                        }
                        read_blocks(bloques);                                       //Cargo en memoria los bloques pedidos
                        return 0;                                                   //Retorno 
                    }
                    else{                                                           //Si tienen el mismo nombre pero no es el archivo final
                        actual = subdirectorios[j];                                 //Entro en subdirectorio y repito proceso
                        break;
                    }
                }
            } 
        }
    }

## Ejericico 11


    find_file_less_size(char* dir, int min_bytes, char* arch_nombre){
        
        Ext2FSInode* inode = inode_for_path(dir);       //Obtengo inodo de dir     

        unsigned short longitud = inode->size;          //Obtengo su tamaño en bytes
    
        int cant_bloques = longitud / BLOCK_SIZE;       //Calculo cantidad de bloques a recorrer

        char* buffer = (char*) malloc(BLOCK_SIZE * 2);

        for(int i = 0; i < cant_bloques -1; i++){
            unsigned int direccion1 = get_block_address(inode, i);
            unsigned int direccion2 = get_block_address(inode, i+1);

            read_block(direccion1, buffer);
            read_block(direccion2, buffer+BLOCK_SIZE);

            int iterador = 0;
            while(iterador < BLOCK_SIZE){
                Ext2FSDirEntry* entrada = (Ext2FSDirEntry*) buffer + iterador;
                if(entrada->file_type == 0){                //Asumo file_type == 0 si es archivo normal
                    Ext2FSInode* inodo = (Ext2FSInode*) entrada->inode;
                    
                    if(inodo->size <= min_bytes && strncmp(entrada->name, arch_nombre, sizeof(arch_nombre)) == 0){
                        
                    }
                }
            }
        }
    }
    