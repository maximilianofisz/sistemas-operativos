semaphore sem1;
semaphore print_finished;

void print_finished_handler() {
    sema_signal(&print_finished);
}

int driver_init() {
    sema_init(&sem1, 1); // Arranca el mutex desbloqueado
    sema_init(&print_finished, 0);
    request_irq(HP_FINISHED_INT, print_finished_handler);
    return IO_OK;
}
int driver_write(void *data, int size) {
    sema_wait(&sem1); // Esperamos nuestro turno para usar el dispositivo
    sema_init(&print_finished, 0);

    int print_status;
    int printer_status;

    int low_ink_detected = 1;

    for (int i = 0; i < 5; i++) {
        OUT(LOC_TEXT_POINTER, data);
        OUT(LOC_TEXT_SIZE, size);
        OUT(LOC_CTRL, START);

        print_status = IN(LOC_CTRL); // LOW_INK si no hay tinta

        if(print_status != LOW_INK){
            low_ink_detected = 0;
            break;
        }
    }

    if(low_ink_detected) {
        sema_signal(&sem1);
        return IO_ERROR;
    }

    if(print_status == PRINTING) { // imprimiendo (si la impresion es lenta espero aca a que me avisen que termino, si es rapida quiza ni entro aca porque print_status == FINISHED)
        sema_wait(&print_finished);
    }

    // si llegue aca, la int ya me aviso que termino de imprimir, doy la señal para la proxima impresion
    sema_signal(&sem1);
    return IO_OK;
}


int driver_remove() {
    free_irq(HP_FINISHED_INT);
    return IO_OK;
}