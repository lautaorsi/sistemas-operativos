### Ejercicio 2
Evita este uso de semaforos la _race condition_?

    x = 0; // Variable compartida
    mutex(1); // Mutex compartido
    while (1) {
        mutex.wait();   //espera a que se libere la variable
        y = x; // Lectura de x  
        mutex.signal(); //libero variable
        if (y <= 5) {   //modifico variable???
            x++;
        } else {
            x--;
        }
    }

Podemos ver que claramente **no** frena la race condition ya que actualiza el valor de la variable sin utilizar el semaforo, entonces si por ejemplo tenemos el caso <br> 
Dos procesos P1 y P2 corriendo en simultaneo y x = 4, el proceso P0 lee el valor de x y justo antes del `if` se ejecuta P1 llegando a modificar x vamos a tener que en P0 x vale 5 pero deberia valer 6 (pues P0 obtuvo de memoria X = 4, luego P1 obtiene ese mismo X = 4 y lo actualiza a X = 5 pero como ya realizó la lectura P0 vuelve a modificar la posicion de memoria de P1 a 5) 


### Ejercicio consultas
    semaforo_t permisoPush = semaforo(N)
    semaforo_t permisoPop = semaforo(0)
    buffer() //aca hacemos el push y pop
    semaforo_t mutexBuffer = semaforo(1);

    proc_q_escribe{
        permisoPush.wait()
        item = generarItem()
        mutexBuffer.wait()
        buffer.push(item)
        mutexBuffer.signal()
        permisoPop.signal()
    }

    proc_q_lee(){
        permisoPop.wait()
        mutexBuffer.wait()
        item = buffer.pop()
        mutexBuffer.signal()
        hacerAlgo(item)
        permisoPush.signal()
    }
    
    