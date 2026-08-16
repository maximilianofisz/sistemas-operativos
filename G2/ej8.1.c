// 8.1
sem_t A = 1;
sem_t B = 0;
sem_t C = 0;

// Proceso A
while (1) {
    A.wait();

    procesoA();

    B.signal();
}
// ----------------------------
// Proceso B
while (1) {
    B.wait();

    procesoB();

    C.signal();
}
// ----------------------------
// Proceso C
while (1) {
    C.wait();

    procesoC();

    A.signal();
}

// No hay inanicion porque es una suerte de round robin, no puede hacer ciclos de esperas


