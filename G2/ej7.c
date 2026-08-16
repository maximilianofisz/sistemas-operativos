sem_t turno[N]; // una coleccion de semaforos para cada proceso

for (int j = 0; j < N; j++) {
    turno[j] = 0;
}

turno[i] = 1; // permitimos solo el de i

// Asumamos que soy el proceso k, con 0 < k < n - 1, en particular k = 25

turno[k].wait();

ejecucion()

turno[(k + 1) % N].signal();
