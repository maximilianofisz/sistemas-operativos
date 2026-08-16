// 9

counter = 0;
mutex = 1;
barrier = 0;

// Ejecutamos N procesos Pi
// Para cada proceso Pi tienen:

sentenciasA();

mutex.wait();
counter++;

if(counter == n) {
    for (int i = 0; i < n; i++) {
        barrier.signal();
    }
}

mutex.signal();

barrier.wait();

sentenciasB();



