#include <stdio.h>   // Library for input/output
#include <string.h>

typedef struct PCB {
    int STAT; // valores posibles KE_RUNNING, KE_READY, KE_BLOCKED, KE_NEW
    int P_ID; // process ID
    int PC; // valor del PC del proceso al ser desalojado
    int RO; // valor del registro R0 al ser desalojado
    int R15; // valor del registro R15 al ser desalojado
    int CPU_TIME; // tiempo de ejecución del proceso
} PCB;


void Ke_context_switch(PCB* pcb_0, PCB* pcb_1) {

    pcb_0->RO = R0;
    ...
    pcb_0->R15 = R15;

    pcb_0->STAT = KE_READY;
    pcb_0->CPU_TIME = pcb_0->CPU_TIME + ke_current_user_time();
    ke_reset_current_user_time();

    set_current_process(pcb_1->P_ID);
    pcb_1->STAT = KE_RUNNING;

    R0 = pcb_1->RO;
    ...
    R15 = pcb_1->R15;
}