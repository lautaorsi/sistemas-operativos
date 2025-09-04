# Scheduling 

### Ejercicio 2

- **P<sub>0</sub>** tiene rafagas cortas de I/O a ciertos dispositivos
- **P<sub>1</sub>** frecuentemente se bloquea leyendo la red
- **P<sub>2</sub>** tiene rafagas prolongadas de CPU y luego escritura a disco

Usar el algoritmo Round Robin seria contraproducente, tendriamos para el proceso P<sub>0</sub> intervalos en los que no responderia. Respecto a P<sub>1</sub> no seria tanto un problema  