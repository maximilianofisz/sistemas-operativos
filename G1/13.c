int result;
void proceso_izquierda() {
    result = 0;
    while (true) {
        while(!send(pid_derecha, &result)) {}
        result = cómputo_muy_difícil_1();
    }
}

void proceso_derecha() {
    while(true) {
        result = cómputo_muy_difícil_2();
        int msg;
        while(!receive(pid_izquierda, &msg)) {}
        int left_result = msg;
        printf("%s %s", left_result, result);
    }
}
