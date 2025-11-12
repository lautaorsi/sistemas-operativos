## Drivers

### Información importante
- Los drivers corren **dentro del contexto** de un proceso
- Los drivers corren con **privilegio maximo**
- Un driver puede ser **ejecutado por muchos procesos** a la vez, generando race conditions


### Funciones base:
- **int driver_init()**: Es ejecutada la primera vez que se carga el driver y nunca más
- **int driver_open()**: Es ejecutada al solicitarse un open
- **int driver_close()**: Es ejecutada al solicitarse un close
- **int driver_read()**: Es ejecutada al solicitarse un read
- **int driver_write()**: Es ejecutada al solicitarse un write
- **int driver_remove()**: Es ejecutada cuando se remueve el driver y nunca más 

### Syscalls útiles
- **void OUT(int IO_address, int data)**: **ESCRIBE** data en el registro IO_address
- **void IN(int IO_address, int data)**: **LEE** el valor almacenado en IO_address
- **int copy_from_user(void\* to, void\* from, int n)**: copia n bytes desde from (memoria usuario) a to (memoria kernel), devuelve 0 si funciona y otro numero si falla.
- **int copy_to_user(void\* to, void \* from, int n)**: copia n bytes desde from (memoria kernel) a to (memoria usuario), devuelve 0 si funciona y otro numero si falla. 
- **int request_irq(int irq, void *handler)**: Asocia interrupción irq a funcion handler
- **int free_irq(int irq)**: Libera la interrupcion del procedimiento asociado



## Sistemas de Archivos



### FAT (File Allocation Table)

Es una tabla que divide la memoria en bloques de un tamaño arbitrario con las siguientes propiedades: <br>
- La tabla siempre debe estar cargada en memoria
- En cada indice *n* podemos tener: 
    - Indice del siguiente bloque del archivo
    - EOF si es el último bloque necesitado por el archivo
    - 0 si esta libre (Es arbitrario, imagino que no va a importar en pseudocodigo)
- El tamaño de la tabla es fijo y proporcional al tamaño del disco, dado por:
    - ``cantidad_indices_tabla = tamaño_disco / tamaño_bloque (arbitrario)``  
    - ``bytes_necesitados = log(cantidad_indices_tabla)/8 `` 
    - ``tamaño_tabla = cantidad_indices_tabla * bytes_necesitados`` 
- Si el archivo es un **directorio** el dato contenido en el bloque sera una lista de *Metadatos* y indice_primer_bloque correspondiente a cada entrada del directorio
    - *Metadatos*: nombre, tamaño, fecha de ´ultimo acceso, etc
- El bloque del directorio root es distinguido del resto 

![Estructura del sistema FAT32](image-1.png)


### Inodos

Es una estructura asignada a cada archivo, con las siguientes propiedades:
- Solo se carga en memoria el inodo a utilizar
- Tiene un esquema de direcciones a bloques de datos con las siguientes opciones
    - Direccionamiento Directo &rarr; Contiene direccion a bloque de datos del archivo
    - Direccionamiento Indirecto Simple &rarr; Contiene direccion a una tabla de direcciones a bloques de datos del archivo
    - Direccionamiento Indirecto Doble &rarr; Contiene direccion a un bloque de datos con direcciones a tablas de tablas de datos del archivo
    - Direccionamiento Indirecto Triple &rarr; Idem a Doble, pero con una tercer tabla (evidentemente)
- Si el archivo es un **directorio** el dato contenido en los bloques sera una lista de tamaño_entrada, nombre e indice_inodo correspondiente a cada entrada del directorio

### Sistema Ext2

El sistema Ext2 es una versión que incorpora la estructura de inodos, donde el **disco esta partido en grupos de bloques** que contienen:
- **Super bloque** &larr; contiene informacion de TODO el sistema (cantidad de inodos, cantidad de bloques, inicio de la tabla de inodos) (1 bloque)
- Descriptores de grupo (n bloques)
- Data Block e Inode Bitmap &larr; informacion de bloques/inodos libres(1 bloque cada uno)
- Inode Table &larr; bloques que contienen los inodos. (m bloques)
- Data Blocks &larr; almacena datos de archivos y directorios (m bloques)

Veamos el paso a paso de como conseguir, a partir de un numero de inodo, su puntero y repasemos algunas cosas importantes de las estructuras:
- **Estructuras**

        struct superblock_t {
            ...
            int inode_size;                         //Tamaño de inodo
            int block_size;                         //Tamaño de bloque
            ...
            usigned int s_blocks_per_group;         //Cantidad de bloques por grupo
            ...
            usigned int s_inodes_per_group;         //Cantidad de inodos por grupo
            ....
        } 

        struct blockgroup_t {
            ...
            unsigned int inode_table;
            ...
        }

        struct Ext2FSInode{
            ...
            unsigned short mode;
            ...
        }

- **Funciones auxiliares**

        Ext2FSBlockGroupDescriptor* block_group(int blockgroup_number);                         //Dado un numero de blockgroup te retorna el descriptor
    
    La verdad que esto se puede hacer manualmente usando el atributo bgt (block group table), pero no lo mencionaron en clase y en el taller te tiran esta función, asi que uso esto. <br> Las otras funciones auxiliares son medio redundantes (solo hacen asserts y las cuentas, pero a nivel pseudocodigo son innecesarias)            

Y supongamos que tenemos el numero de inodo en la variable ``inode_number``

1) **Obtengamos el blockgroup** en el que se encuentra nuestro inodo

        int inodos_por_grupo = superblock->s_inodes_per_group;                                  //Asumamos que tenemos el puntero al superbloque
        int numero_blockgroup = inode_number / inodos_por_grupo;                                //Calculamos numero de blockgroup
        blockgroup_t* puntero_blockgroup = block_group(numero_blockgroup)                       //Obtenemos puntero de la tabla de descriptores de blockgroups

2) Ahora busquemos en que **bloque de la *Inode Table*** está el inodo

        int inodos_por_bloque = superblock->block_size / superblock->inode_size;                //Cuantos inodos hay por bloque
        int indice_bloque = inode_number / inodos_por_bloque;                                   //Numero de bloque que contiene al inodo
        int offset_inodo = inode_number % inodos_por_bloque;                                    //Indice de inodo dentro de bloque
        
3) Armemos un buffer y **carguemos el bloque en memoria**, usando el puntero al blockgroup

        void* buffer = malloc(superblock->block_size);                                          //Armamos buffer de tamaño de bloque
        read(superblock->inode_table[indice_bloque], buffer, superblock->block_size);           //Traemos bloque

4) **Busquemos y retornemos el inodo** dentro del bloque
        
        inodo_t* puntero_inodo = buffer[offset_inodo];                                          //Usamos offset para encontrar el inodo dentro de la lista
        free(buffer);
        return puntero_inodo

5) Fin! Es importante notar que con esto solamente obtuvimos el puntero del inodo que **no se encuentra en memoria**, si quisieramos traerlo:

        void* buffer = malloc(blockgroup->inode_size);                                          //Armamos puntero
        read(puntero_inodo, buffer, blockgroup->inode_size);                                    //Leemos de disco
        return *puntero_inodo;                                                                  //Retornamos estructura

*Observación, tiene memory leaks (no liberamos el buffer del inodo)*

### Links
- Hard Links: Nos permiten crear otro nombre para el archivo, sin duplicar los datos &larr; NO PUEDE HACERSE SOBRE DIRECTORIOS
    - Solo es posible en inodos, ya que el nombre no aparece en el mismo y podemos simplemente referenciar al inodo con otro nombre 
    - Cada inodo lleva un contador de hard links, luego el archivo solo se borra si todos sus hard links fueron eliminados

- Symbolic Links: Crea un archivo (y su inodo) que contiene el path al archivo linkeado &larr; Permite hacerlo entre distintos sist de archivos
    - Solo es posible en inodos
    - No llevan contadores de symbolic links, si tenemos un symbolic link en el sistema y se borra el original tenemos problemas



## Seguridad

### Stack frames

```
    
--- direcciones bajas ---

+++
variables locales
argumentos                  STACK FUNCION LLAMADA
saved ebp
+++

+++
Ret addr
parametros                  STACK FUNCION LLAMADORA
...
+++

--- direcciones altas ---

```

### Buffer overflows




