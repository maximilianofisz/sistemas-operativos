semaphore mutex;

int driver_init() {
    sema_init(&mutex, 1);
}

int driver_read(int *data) {
    sema_wait(&mutex);
    int status = IN(BTN_STATUS);
    while(!IN(BTN_STATUS)[0]) {} // solo pasas de aca cuando la tecla haya sido pulsada (busy waiting)
    status[1] = 0;
    OUT(BTN_STATUS, status);
    sema_signal(&mutex);

    copy_to_user(data, BTN_PRESSED, sizeof(BTN_PRESSED));
    return IO_OK;
}