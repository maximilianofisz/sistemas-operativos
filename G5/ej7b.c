semaphore sem1;
semaphore drive_operation;
semaphore timer;

void operation_finish_handler() {
    sema_signal(&drive_operation);
}

void timer_handler() {
    sema_signal(&timer);
}

int driver_init() {
    sema_init(&sem1, 1); // Arranca el mutex desbloqueado
    sema_init(&drive_operation, 0);
    request_irq(6, operation_finish_handler);
    request_irq(7, timer_handler);
    return IO_OK;
}
int driver_write(int sector, void *data) {
    sema_wait(&sem1); // Esperamos nuestro turno para usar el dispositivo

    int device_status = IN(DOR_STATUS); // veamos el estado del disco a ver si esta apagado o prendido

    if(!device_status) { //Si esta apagado, lo prendemos
        OUT(DOR_IO, 1);
        sema_init(&timer, 0);

        sema_wait(&timer); // yo diria que con un wait no te basta porque podria haber habido una int hace 1ms pero el comando de encendido se envio recien atras hace por ejemplo 5ms
        sema_wait(&timer);
    }


    int number_of_sectors_per_track = cantidad_sectores_por_pista(); //LBA a lo que nos piden
    int track_to_write_to = sector / number_of_sectors_per_track;
    int sector_to_write_to = sector % number_of_sectors_per_track;

    OUT(ARM, track_to_write_to); // queremos escribir en track_to_write_to
    OUT(SEEK_SECTOR, sector_to_write_to); // queremos escribir en sector_to_write_to

    sema_wait(&drive_operation);
    escribir_datos(data);
    sema_wait(&drive_operation);

    sema_init(&timer, 0);

    OUT(DOR_IO, 0); // secuencia de apagado
    sema_wait(&timer);
    sema_wait(&timer);
    sema_wait(&timer);
    sema_wait(&timer);
    sema_wait(&timer);
    sema_signal(&sem1);

    return IO_OK;
}


int driver_remove() {
    free_irq(6);
    free_irq(7);
    return IO_OK;
}