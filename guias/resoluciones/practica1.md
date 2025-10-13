# Practica 1: Procesos y API del SO 


### Ejercicio 1

Debemos guardar una imagen del estado del procesador antes de realizar el cambio, almacenar esa informacion para la tarea que estaba siendo ejecutada (en el **PCB o Process Control Block**) y luego modificar el estado del procesador (esto es: sus registros, pc, sp, etc). <br>
Ademas, el quantum debera ser actualizado acorde a aquel determinado por la politica de scheduling para el proceso a ejecutar. 

### Ejercicio 2
Implementar la rutina encargada de realizar el cambio de contexto entre dos procesos (cuyos programas ya han sido cargados en memoria) debido a que el primero ha consumido su quantum. <br>

    void ke_context_switch(PCB* pcb_0, PCB* pcb_1){
        
        pcb_0[R0...R15] .=. R0...R15;       ## guardamos los valores de registros en la pcb0
        R0...R15 .=. pcb_1[R0...R15];       ## actualizamos los registros con la pcb1
                                            ## (en un muy pseudo-codigo)

        set_current_process(pcb_1.P_ID);    ## actualizamos el process id al del proceso 1

        ke_reset_current_user_time();       ## restarteamos el quantum, asumo que el scheduling usa un sistema que reparte a todos 
                                            ## la misma cantidad de tiempo o que usa el process id asignado como siguiente a ejecutarse para asignar el quantum correspondiente

        ret();                              ## desapilamos pila y reemplazamos el PC
    }


### Ejercicio 4

TODO

### Ejercicio 5
[Resuelto en practica1.c](./practica1.c)

### Ejercicio 6
[Resuelto en practica1.c](./practica1.c)

### Ejercicio 8

No son iguales:
- Caso hijo entra en el 2do if e incrementa en 3 la variable "dato" local a su contexto.
- Caso padre entra en el 3er if y no incrementa el dato, manteniendose en 0.

### Ejercicio 9
[Resuelto en practica1.c](./practica1.c)

### Ejercicio 10
[Resuelto en practica1.c](./practica1.c)



### Ejercicio 20

    //Servidor 


