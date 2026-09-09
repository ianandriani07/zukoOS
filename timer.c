#include "timer.h"
#include "kernel.h"
#include "scheduler.h"

extern queue_t ReadyQueue;
extern uint8_t task_running;
extern uint16_t *task_running_tos;

void config_timer_0(void)
{
    T1CONbits.TCS = 0;          // Clock interno 
    T1CONbits.TCKPS = 0b01;     // Preescaler 1:8
    IFS0bits.T1IF = 0;          // Flag
    IEC0bits.T1IE = 1;          // Atividação da interrupção por timer
    IPC0bits.T1IP = 4;          // Prioridade
    PR1 = 1999;    
}

void __attribute__ ((interrupt, no_auto_psv, naked)) _T1Interrupt()
{
    SAVE_CONTEXT();
    
    IFS0bits.T1IF = 0;
    
    ReadyQueue.tasks[task_running].sp = task_running_tos;
    ReadyQueue.tasks[task_running].task_state = READY;
    
    task_running = scheduler();
    task_running_tos = ReadyQueue.tasks[task_running].sp;
    ReadyQueue.tasks[task_running].task_state = RUNNING;
    
    RESTORE_CONTEXT();    
}

