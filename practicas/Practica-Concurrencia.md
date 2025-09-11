### Ejercicio 1

Proceso productor P que puede producir() y dos procesos consumidores C1, C2 que hacen consumir1() y consumir2() respectivamente. Se desea sincronizarlos tal que las secuencias de ejecucion sean: producir, producir, consumir1, consumir2, producir, producir, consumir1, consumir2, ...

    semaforo_t permiso_c1 = sem(0)  //Queremos que C1 empiece recien cuando P() le de el okay
    semaforo_t permiso_c2 = sem(0)  //Queremos que C2 empiece recien cuando C1() le de el okay
    semaforo_t permiso_P = sem(1)   //Queremos que P arranque de una

    void P(){
        while(1){
            permiso_p.wait()        
            producir()
            producir()
            permiso_c1.signal()
        }
    }

    void C1(){
        while(1){
            permiso_c1.wait()
            consumir1()
            permiso_c2.signal()
        }    
    }

    void C2(){
        while(1){
            permiso_c2.wait()
            consumir2()
            permiso_P.signal()
        }
    }


### Ejercicio 2

Se tienen 2 procesos A y B 
- El proceso A debe ejecutar A1() y luego A2() 
- El proceso B debe ejecutar B1() y despues B2() 

En cualquier ejecucion, A1() tiene que ejecutarse antes que B2()

    //no usar atomico para consulta + asignacion pq se podria hacer en pasos separados
    permiso_B2 = sem(0)


    void A(){
        A1()
        permiso_B2.signal()
        A2()
    }

    void B(){
        B1()
        permiso_B2.wait()
        B2()
    }


