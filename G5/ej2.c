semaphore mutex;

int driver_init() {
    sema_init(&mutex, 1); // Inicializamos un semaforo "desbloqueado", se puede entrar a la zona critica en 1
}

int driver_read(int *data) {
    sema_wait(&mutex); // Esperamos a que este "desbloqueado"
    int current_time = IN(CHRONO_CURRENT_TIME);
    sema_signal(&mutex); // Desbloqueamos
    copy_to_user(data, &current_time, sizeof(current_time));
    return IO_OK;
}

int driver_write(int *data) {
    sema_wait(&mutex); // Esperamos a que este "desbloqueado"
    OUT(CHRONO_CTRL, CHRONO_RESET);
    sema_signal(&mutex);
    return IO_OK;
}