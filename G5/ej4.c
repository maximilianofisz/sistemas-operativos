semaphore pressed;

void handler() {
    sema_signal(&pressed); // Cuando el dispositivo manda la int le permitimos el paso a quien este esperando en el wait del read
}

int driver_init() {
    sema_init(&pressed, 0); // Arranca el mutex desbloqueado
    if(request_irq(7, handler) == IRQ_ERROR) { // registramos la funcion handler a la interrupcion 7
        return IO_ERROR;
    }
    OUT(BTN_STATUS, BTN_INT); // le decimos al dispositivo que puede arrancar a mandar pulsaciones
    return IO_OK;
}

int driver_read(int *data) {
    sema_wait(&pressed); // Esperamos a recibir la interrupcion
    OUT(BTN_STATUS, BTN_INT); //Le aviso al dispositivo que tiene que clearearse y mandar una nueva interrupcion?
    copy_to_user(data, &BTN_PRESSED, sizeof(BTN_PRESSED));
    return IO_OK;
}

int driver_remove() {
    free_irq(7);
    return IO_OK;
}