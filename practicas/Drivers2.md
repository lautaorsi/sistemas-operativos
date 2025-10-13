# Ejercicio Brazo robot

Armamos 3 drivers: **Brazo, Movimiento y Lector**

## Movimiento

### Registros
- (r/w) CORD_X 
- (r/w) CORD_Y
- (w) STATUS

### Interrupciones
- POS_FIN  
       
### API
```
struct Posicion{
    int x;
    int y;
}
```
```
void driver_init(){
    sema_init(sem,0);
    irq_register(POS_FIN, handler)
}
```
```
int driver_write(*data){
    posicion_t p;
    copy_from_user(data, p, sizeof(p));
    OUT(CORD_X, p.x);
    OUT(CORD_Y, p.y);
    OUT(STATUS, start);
    sem.wait();     
    return 1;
}
```
```
void handler(){
    sem.signal();
}
```

## Brazo

### Registros
- POS
- EXTENT
- MANO

### API

```
struct brazo_op_t{
    int pos;
    enum EXTENT = {CONTRAER, EXTENDER}
    enum MANO = {SOLTAR, AGARRAR}
}

```

```
int driver_write(*data){
    brazo_op_t brazo_op;
    copy_from_user(data, brazo_op, sizeof(...));
    OUT(POS, brazo_op.pos)
    OUT(EXTENT, brazo_op.extent)
    OUT(MANO, brazo_op.mano)
}
```


## Lector

### Registros
- Codigo

### API

```
int driver_read(*data){
    int codigo = IN(CODIGO);
    copy_to_user(data, &codigo, sizeof(codigo));
}
```


## Usuario


```
int main(){
    int brazo = open("/dev/brazo", w);
    int mov = open("/dev/movimiento", w);
    int lector = open("dev/lector", r);
    while(1){
        paquete_t p = siguiente_paquete();
        write(mov, {p.x, p.y}, 2*sizeof(int));
        for(inti = 0; i < 10; i++){
            write(brazo, {i, EXTENDER, AGARRAR}, sizeof(brazo_op_t));
            write(brazo, {i, CONTRAER, AGARRAR}, sizeof(brazo_op_t));
            int cod;
            read(lector, &cod, sizeof(cod));
            if(cod == p.cod){
                write(mov, {0,0}, 2*sizeof(int));
                break;
            }
            else{
                write(brazo, {i, EXTENDER, AGARRAR}, sizeof(brazo_op_t));   
                write(brazo, {i, EXTENDER, SOLTAR}, sizeof(brazo_op_t));            //Hacemos movier -> soltar en 2 pasos distintos por las dudas
                write(brazo, {i, CONTRAER, SOLTAR}, sizeof(brazo_op_t));
            }
        }

    }
}
```




