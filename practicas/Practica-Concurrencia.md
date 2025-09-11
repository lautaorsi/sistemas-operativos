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