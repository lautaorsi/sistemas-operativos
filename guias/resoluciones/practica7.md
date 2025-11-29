# Protección y Seguridad

## Ejercicio 3

### a)
El problema está en la declaracion de la variable <br>
``char nombre[80];`` <br>
Y la correspondiente lectura del input del usuario pasado por stdin <br>
``gets(nombre);``

### b)

Una vez que tenemos un buffer overflow no hay limites respecto a lo que se puede hacer (si sabemos como hacerlo), podriamos pisar variables apiladas debajo en el stack, insertar codigo malicioso y modificar la direccion de retorno, etc

### c) 

Si, podriamos insertar codigo malicioso al principio de nuestro buffer y luego extender el dato hasta pisar el registro de retorno, modificandolo por la direccion en la que se encuentra el principio de nuestro buffer

### d)

No, podemos seguir rompiendo cosas ya que el problema está en la carga del dato a nuestr buffer, podemos aplicar la misma idea que hicimos en el ``printf()`` para nuestra funcion ``void saludo(void)``

## Ejercicio 4

### a)
La pila de login quedaría del estilo:

- Parte alta (direcciones bajas)
- name [32]
- pass [32]
- realpass [32]
- direccion retorno
- Parte baja (direcciones altas)

Tenemos dos fgets, <br> El primer caso el usuario solo puede pisar informacion de `credential.name` y `credential.pass` ya que tenemos definido leer solo 64 bytes de información (por el `sizeof(user)`) <br> El segundo caso por otro lado si nos permite pisar el `realpass` ya que leemos como mucho 64 bytes y esto es mas que los 32 que otorga `credential.pass`.

### b)
Sencillamente tendriamos que pasar como input algo del estilo

    admin
    ABCD [28 bytes de basura] ABCD

Con esto hacemos que loggee al usuario admin y pisamos realpass para que cuando haga la comparación sean iguales (esto funciona ya que el 2do fgets esta despues de fetchear la contraseña real de la base de datos, por lo tanto podemos sobreescribirlo mediante el overflow)     



## Ejercicio 16

El codigo pareceria hacer algo del estilo:
    
- Arma una variable _fmt_ para armar el formato que usara scanf a la hora de leer la clave
- A _fmt_ le pone el largo de la clave como string, de largo maximo 4 digitos
- Convierte el input a int y lo guarda en max_size **Aca se presenta la vulnerabilidad**
- Completa el formato para que pueda ser usado por scanf
- Lee max_size bytes (erroneo por overflow) de stdin y eso lo guarda en clave 

Entonces nuestros problemas son:
- Ya que el dato se almacena en un _unsigned char_ (1 byte = 8 bits), el maximo numero representable es 255, pero al ser de 4 digitos vamos a tener un overflow si es mayor a 255 
- El _fmt_ sigue teniendo el largo real de la clave pero _clave_ solo mide lo que permita unsigned char, entonces si tuvimos un int overflow vamos a generar un buffer overflow (sobre el buffer _clave_) 

Y nuestras soluciones:
- Usar size_t para max_size 
- Agregar stack canaries para identificar buffeer overflows (esto no es infalible ya que al ser a nivel compilador solo lo vamos a detectar si se sale del stack del contexto actual, pero si pisamos fmt por ejemplo probablemente no lo detecte. Todo depende de la implementacion del canary)

## Ejercicio 17


La funcion algo_asi_si en el ejemplo deberia imprimir <br>
hola 127 chau! <br>
Y retornar "chau!" <br>
Mientras que algo_asi_no va a fijarse que la cadena sea menor a 127 bytes y escribir en buffer "echo 0 asi si?\n" (el 0 por ser un "hola")<br>

El problema entonces no está en estas funciones, en realidad está en el uso del printf(cadena) que es lo que retornan ambas funciones (no se si la idea del resto del codigo era confundir o que, pero no le veo fallas fuera de eso). Es un format string exploit estandar, podemos obtener datos del stack usando %x para obtener sus punteros y en base a eso -y asumiendo que no esta implementado la randomizacion del stack- podemos pisar variables (como _format_, _MAX-SIZE_), direcciones de retorno, etc.

## Ejercicio 18

Podemos hacer una inyeccion de shell en el echo escapando de las comillas simples <br> 
"echo ’ %s’ > %s" se puede escapar cuando el primer %s es " 'hola ; rm rf unaCarpeta' echo unaFrase". Con este exploit podriamos hacer que se guarde la frase y se cree el archivo correctamente, pero al mismo tiempo ejecutamos un comando malicioso entonces nadie se podria dar cuenta (en un principio) de que hubo un ataque

## Ejercicio 19

El problema pareceria estar cuando se hace la cuenta para _saldo_ `creditos - costo`, al ser un **unsigned int** si _costo_ es mayor a _creditos_ va a dar un numero negativo y como es sin signo va a tener un int overflow que nos retorne un numero gigante.

## Ejercicio 20

Hace un gets() cuando va a confirmar la contraseña, que no limita la cantidad de bytes leidos en stdin y como el orden de declaracion es numeroDeUsuario, password, passwordconfirmacion el stack tiene la pinta:

- passwordConfirmacion (250 bytes)
- password (250 bytes)
- numeroDeUsuario (sizeof(int) bytes)

Entonces podriamos pisar tanto la password como el numeroDeUsuario, esto basicamente le da permisos al atacante a cambiar la contraseña de cualquier usuario cuyo numero conozca

## Ejercicio 21
Las variables son:

    int a (4 bytes)
    char buffer (16 bytes)
    int b (4 bytes) 

Con esto vemos que:

- Si el codigo esta optimizado y las variables que no son utilizadas no se agregan al stack quedando unicamente el buffer, que cuando hagamos el memcpy va a tener un overflow y pisará datos.
- Si no esta optimizado y las variables se agregan al stack indiferentemente de si se usan o no entonces cuando hagamos el cpy los 4 bytes que causan el overflow simplemente van a pisar la variable a (recordemos que el orden del stack es inverso al orden de declaracion) entonces si bien sigue siendo un error y definitivamente no es un codigo bien hecho no va a tener grandes complicaciones (a menos que despues se haga algo con esa variable modificada)