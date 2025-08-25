# Parte 1: Linux terminal & Bash scripting
## Ejercicio 1 (Comandos de información)
1. El comando **whoami** imprime el nombre de usuario asociado al ID de usuario "efectivo", que basicamente determina los permisos que el proceso tiene actualmente
2. El comando **uname**  imprime la informacion del sistema (Linux en este caso), agregarle **-a** nos da toda la informacion (mas detallado, nos da el nombre de maquina, fecha y hora, version del SO, y un par de datos extra)
3. El comando **id** imprime el id real y efectivo tanto del usuario como del grupo (si x ejemplo corremos algo con root momentaneamente, el id efectivo es de root pero el real es de user) **_CHEQUEAR_**
4. El comando **ps** imprime una "imagen" de los procesos activos con el mismo id de usuario efectivo que aquel que corre el comando -mostrando el id de proceso, la terminal asociada, el tiempo de CPU acumulado y el nombre de ejecutable-, agregar **-e** nos muestra todos los procesos en el sistema 
5. El comando **top** nos muestra una vista dinamica en tiempo real de -entre otros- informacion sobre el sistema, lista de procesos y threads manejados por el kernel, procentaje de CPU y MEM que usan y usuario que lo ejecuta, agregar **-n 10** limita la cantidad de iteracions o _frames_ que **top** producira antes de frenar (sin esto va a seguir produciendo frames hasta que el usuario lo corte manualmente).

## Ejercicio 2 (inspeccionar archivos y directorios)
1. Ejecutar **ls /** nos imprime la lista de archivos y directorios "anteriores" a home
2. Para inspeccionar el directorio **/bin** debemos realizar el siguiente comando desde _home_ **ls /bin**
3.  - **-a**: no ignora las entradas que empiezan con .
    - **-d**: lista los directorios, no sus contenidos 
    - **-h**: lo hace mas leíble cuando se combina con -l o -s, imprimiendo tamaños como 1K, 234M, 2G, etc.
    - **-l**: da mas informacion sobre cada archivo/directorio
    - **-S**: ordena por tamaño de archivo de forma DESC
    - **-t**: ordena por tiempo, poniendo los mas nuevos primero
    - **-r**: cuando ordena hace en orden inverso

## Ejercicio 4 (Manejo de archivos y directorios)
3. La diferencia entre **rmdir** y **rm -r** es que **rmdir** permite unicamente borrar directorios que estan vacíos, mientras que **rm -r** nos permite borrar directorios junto con todos sus elementos internos. (**-r** indica que se eliminara todo de forma recursiva)

## Ejercicio 5 (Permisos)
1. user: read, write | group: read | other: read
*Usar chmod (u,g,o)(+-)(r,w,x) para modificarlos*

## Ejercicio 6 (Archivos de texto: ver contenido)
*Cat muestra contenido, si es muy largo solo muestra el final* <br>
*More muestra el principio, podes ir bajando por paginas con el espacio* <br>
*Less muestra el principio, podes subir y bajar, apretar q para salir* 

## Ejercicio 7 (Archivos de texto: extraer lineas y campos)
*grep (patron) (archivo) nos permite buscar las lineas que coincidan con (patron)*

1. opciones: 
    - **n**: retorna el indice de la linea que matchea
    - **c**: retorna un contador de matches en vez de los matcheos 
    - **i**: hace que sea case insensitive (no importa mayuscula o minuscula)
    - **v**: invierte el match (muestra aquellas que no matchean) 
    - **w**: el match tiene que formar una palabra entera
