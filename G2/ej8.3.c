// 8.3
sem_t productor = 1;
sem_t consumidor = 0;

int counter_consumidores = 0; 
semt_t mutex = 1;

// Proceso A
while (1) {
    productor.wait();

    procesoA();
    consumidor.signal();
    consumidor.signal();
}
// ----------------------------
// Proceso B
while (1) {
    consumidor.wait();

    procesoB();

    mutex.wait();

    counter_consumidores++;
    if(counter_consumidores == 2) {
        counter_consumidores = 0;
        productor.signal();
    }

    mutex.signal();
}
// ----------------------------
// Proceso C
while (1) {
    consumidor.wait();

    procesoC();

    mutex.wait();
    
    counter_consumidores++;
    if(counter_consumidores == 2) {
        counter_consumidores = 0;
        productor.signal();
    }

    mutex.signal();
}

// No hay inanicion porque es una suerte de round robin, no puede hacer ciclos de esperas


