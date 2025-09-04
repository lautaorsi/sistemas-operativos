# Scheduling 

### Ejercicio 2

- **P<sub>0</sub>** tiene rafagas cortas de I/O a ciertos dispositivos
- **P<sub>1</sub>** frecuentemente se bloquea leyendo la red
- **P<sub>2</sub>** tiene rafagas prolongadas de CPU y luego escritura a disco

Usar el algoritmo Round Robin seria contraproducente, tendriamos para el proceso P<sub>0</sub> intervalos en los que no responderia. Respecto a las otras 2, parecen ser procesos mas lentos o extensos entonces requerirían varias iteraciones del RR para producir resultados reales. <br>
Propongamos entonces usar un algoritmo de prioridades, podriamos utilizar uno que asigne niveles, ponienod a P<sub>0</sub> como prioridad 0, P<sub>1</sub> como prioridad 2 y P<sub>2</sub> como prioridad 1. Esto nos asegura que el proceso mas "inmediato" (_P<sub>0</sub>_) tendra la prioridad y no producirá starvation, el proceso extenso -y aparentemente mas importante- (_P<sub>2</sub>_) tomaria el control luego de que P<sub>0</sub> terminara de ejecutarse y luego otorgariamos tiempo a P<sub>1</sub> que parece ser el menos prioritario. Podriamos ademas darles un quantum, cuestión de no starvear a P<sub>1</sub> o incrementar la prioridad en base al tiempo, usando RR entre las tareas de misma prioridad.  