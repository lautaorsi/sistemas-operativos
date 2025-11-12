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
