semaphore init;
semaphore read[3];
char input_mem[3][100];
char buffer_lectura[3][1000];
atomic_int buffer_start[3];
atomic_int buffer_end[3];
boolean procesos_activos[3];


void key_pressed_handler() {
    int data = IN(KEYB_REG_DATA);

    int keycode = data & 0x3FFF; // bits 0..13
    int p = (data >> 14) & 0x3; // bits 14..15

    char c = keycode2ascii(keycode);

    boolean success;

    // ahora vamos a guardar en el/los buffer/s segun el id la traduccion del keycode usando keycode2ascii(...)
    if (p == 0) {
        success = write_to_all_buffers(c);

        if (success) {
            for(int i = 0; i < 3; i++) {
                sema_signal(&read[i]);
            }
        }

    }
    else {
        // intentamos guardar en el buffer id, chequeos etc etc
        success = write_to_buffer(p - 1, c);
        
        if(success) {
            sema_signal(&read[p - 1]);
        }
    }

    if (success) {
        OUT(KEYB_REG_CONTROL, READ_OK);
    }
    else {
        OUT(KEYB_REG_CONTROL, READ_FAILED);
    }
}

void driver_load() {
    sema_init(&read[0], 0);
    sema_init(&read[1], 0);
    sema_init(&read[2], 0);
    sema_init(&init, 1);
    mem_map(INPUT_MEM_0, input_mem[0], 100);
    mem_map(INPUT_MEM_1, input_mem[1], 100);
    mem_map(INPUT_MEM_2, input_mem[2], 100);

    for (int i = 0; i < 3; i++) {
        procesos_activos[i] = 0;
        buffer_start[i] = 0;
        buffer_end[i] = 0;
    }

    request_irq(IRQ_KEYB, key_pressed_handler);
}

void driver_unload() {
    mem_unmap(INPUT_MEM_0);
    mem_unmap(INPUT_MEM_1);
    mem_unmap(INPUT_MEM_2);
    free_irq(IRQ_KEYB);
}


int driver_open() {
    sema_wait(&init);
    // buscamos el primer id inactivo y lo asignamos
    for (int i = 0; i < 3; i++) {
        if(!procesos_activos[i]) {
            procesos_activos[i] = 1;

            // Le avisamos al teclado que levantamos un nuevo proceso
            OUT(KEYB_REG_STATUS, APP_UP);
            OUT(KEYB_REG_AUX, i + 1);

            sema_signal(&init);
            return i;
        }
    }

    sema_signal(&init);
    return -1;
}

int driver_close(int id) {
    sema_wait(&init);
    procesos_activos[id] = 0;

    // Le avisamos al teclado que desconectamos un proceso
    OUT(KEYB_REG_STATUS, APP_DOWN);
    OUT(KEYB_REG_AUX, id + 1);
    
    sema_signal(&init);
    return IO_OK;
}

int driver_read(int id, char* buffer, int length) {

    while(get_buffer_length(id) < length) { 
        sema_wait(&read[id]); //Si no hay suficientes caracteres, me duermo
    }

    copy_from_buffer(id, buffer, length);

    return IO_OK;

}

int driver_write(char* input, int size, int proceso) {
    copy_from_user(input_mem[proceso], input, size);
    return size;
}