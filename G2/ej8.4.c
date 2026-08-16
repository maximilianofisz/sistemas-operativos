// 8.2
sem_t A = 1;
sem_t B = 0;
sem_t C =  0;

turno_de_C = 1;
counter_Bs = 0;
mutex = 1;

// Proceso A
while (1) {
    A.wait();

    procesoA();

    if(turno_de_C) {
        turno_de_C = 0;
        C.signal();
    }
    else {
        turno_de_C = 1;
        B.signal();
        B.signal();
    }
}
// ----------------------------
// Proceso B
while (1) {
    B.wait();

    procesoB();

    mutex.wait();
    counter_Bs++;
    if (counter_Bs == 2) {
        counter_Bs = 0;
        A.signal();
    }
    mutex.signal();
}
// ----------------------------
// Proceso C
while (1) {
    C.wait();

    procesoC();

    A.signal();
}

// No hay inanicion porque es una suerte de round robin, no puede hacer ciclos de esperas


