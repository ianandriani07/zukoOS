#include "kernel.h"
#include <stdio.h>

// Variáveis globais
queue_t ReadyQueue;
uint8_t task_running = 0;
uint16_t *task_running_tos = NULL;

void create_task(callback task, uint8_t prior)
{
    tcb_t *new_task = &ReadyQueue.tasks[ReadyQueue.queue_size];
    
    // Preencher a tcb
    new_task->task_id       = ReadyQueue.queue_size+1;
    new_task->task_func     = task;
    new_task->task_prior    = prior;
    new_task->task_state    = READY;
    
    // Ajustar o SP
    new_task->sp            = &new_task->task_stack[0];
    
    // Inicializar a pilha
    init_stack(new_task);
    
    // Insere na fila de aptos    
    ReadyQueue.queue_size++;
}

void init_stack(tcb_t *task)
{
    uint16_t pc_low, pc_high, *sp;
    
    sp = task->sp;
    
    pc_low  = (uint16_t)task->task_func;
    pc_high = 0x0000;
    
    *sp++ = pc_low;
    *sp++ = pc_high;
    
    // Inicializar os Ws (W0 até W14)
    for (int i = 0; i < 15; i++) {
        *sp++ = 0x0000;
    }
    
    
}