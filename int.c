#include "int.h"
#include "kernel.h"
#include "scheduler.h"


extern queue_t ReadyQueue;
extern uint8_t task_running;
extern uint16_t *task_running_tos;


void config_int0()
{
    IEC0bits.INT0IE = 1;
    IFS0bits.INT0IF = 0;
    IPC0bits.INT0IP = 4;
}

void __attribute__ ((interrupt, no_auto_psv, naked)) _INT0Interrupt()
{
    SAVE_CONTEXT();
    
    IFS0bits.INT0IF = 0;
    
    // Tarefa que está deixando a CPU
    ReadyQueue.tasks[task_running].sp = task_running_tos;
    //ReadyQueue.tasks[task_running].task_state = READY;
    
    task_running = scheduler();
    
    // Tarefa que irá assumir a CPU
    task_running_tos = ReadyQueue.tasks[task_running].sp;
    ReadyQueue.tasks[task_running].task_state = RUNNING;
    
    RESTORE_CONTEXT();    
}
