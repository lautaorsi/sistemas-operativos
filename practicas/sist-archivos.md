## Ejercicio Directorios en FAT

lectura = traer de disco a memoria
**Importante: Asumimos que todo archivo y directorio buscado se encuentra en el primer bloque de directory entries correspondiente al padre**

- Traer bloque root (**1 lectura**)
- Buscar directory entry "home"
- Traer bloque home (**1 lecutra**)
- Buscar directoriy entry "el"
- Traer bloque "el" (**1 lectura**)
- Buscar directory entry "parcial.avi" 

Una vez hecho eso queremos traer solamente los bytes [3000...3083], del archivo parcial.avi cada bloque son 2k bytes, entonces:

1 bloque = [0...2047] <- Acá no estan los bytes 3000...3083

2 bloque = [2048...4095] <- Está en el segundo bloque

Luego:

- Buscar direccion de bloque 2 en FAT

- Leo ese bloque (**1 lectura**)


## Ejercicio Directorios en Ext2

FSA: (Buscar /quiero.txt, bytes [0-5]. [12-17])

- Miro inodo root
- Busco 1er bloque de datos de root (**1 lectura**)
- Leer bloque 1 root
- Busco bloque de inodo quiero.txt (como ya estan en memoria los inodos por consigna, no es lectura)
- Miro inodo quiero.txt
- Busco direccion bloque 1 de quiero.txt
- Leo bloque 1 quiero.txt (**1 lectura**)

FSB: (Buscar /home/aprobar.txt)

- Miro inodo root
- Busco 1er bloque de datos de root (**1 lectura**)
- Leer bloque 1 root
- Busco bloque de inodo home (como ya estan en memoria los inodos por consigna, no es lectura)
- Miro inodo home
- Busco direccion bloque 1 de home
- Busco entrada inodo aprobar
- Miro inodo A
- Leer bloque indireccion
- Buscar bloque que cubra bytes [6500-6600] 

- Buscar bloque que cubra bytes [3500000]   




## Links

### Hard links

Si hacemos *ln p1.XXX p2.XXX* creamos una referencia nueva al puntero asignado por p1.XXX. <br>
El inodo tiene un contador **hard_link_count** que mantiene un recuento de los links, el inodo solo desaparece si **hard_link_count == 0** 

- Se genera una referencia al numero de inodo.

- Cons: Los hard links no pueden hacerse sobre directorios o entre distintos file systems.

*Dato: Hacer rm es hacer unlink*

### Symbolic Links

- Genera un archivo de tipo enlace simbólico (con su propio inodo) que almacena el path original

- Permiten referenciar directorios en otros sistemas de archivos.

- Resolver link = resolver ruta.

- Cons: Si borro el archivo original el link simbolico sigue existiendo pero apunta a basura.

**EN EL INODO DEL LINK SIMBOLICO NO SE GUARDA LA DIRECCIÓN A LA REFERENCIA, SE GUARDA UN PUNTERO AL BLOQUE QUE LA CONTIENE (LA DIAPO ESTA MAL)**

