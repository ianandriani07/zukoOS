#include "sync.h"
#include "int.h"
#include <xc.h>


// Variáveis globais
extern queue_t ReadyQueue;
extern uint8_t task_running;
extern uint16_t *task_running_tos;


void sem_init(sem_t *s, uint8_t value)
{
    s->sem_count        = value;
    s->s_queue_input    = 0;
    s->s_queue_output   = 0;
}

void sem_wait(sem_t *s)
{
    __builtin_disable_interrupts();
    
    s->sem_count--;
    
    if (s->sem_count < 0) {
        // Bloqueia a tarefa
        s->sem_queue[s->s_queue_input] = task_running;
        s->s_queue_input = __builtin_modsd(s->s_queue_input+1, MAX_TASKS);
        //s->s_queue_input = (s->s_queue_input+1) % MAX_TASKS;
        ReadyQueue.tasks[task_running].task_state = BLOCKED;
        CONTEXT_SWITCH();        
    }
    
    __builtin_enable_interrupts();    
}

void sem_post(sem_t *s)
{
    __builtin_disable_interrupts();
    
    s->sem_count++;
    
    if (s->sem_count <= 0) {
        ReadyQueue.tasks[s->sem_queue[s->s_queue_output]].task_state = READY;
        s->s_queue_output = __builtin_modsd(s->s_queue_output+1, MAX_TASKS);
        //s->s_queue_output = (s->s_queue_output+1) % MAX_TASKS;
        ReadyQueue.tasks[task_running].task_state = READY;
        CONTEXT_SWITCH(); 
    }
    
    __builtin_enable_interrupts();    
}


void mutex_init(mutex_t *m)
{
    m->owner = UINT8_MAX;
    m->queue_input = 0;
    m->queue_output = 0;
    m->queue_count = 0;
}

void mutex_lock(mutex_t *m)
{
    __builtin_disable_interrupts();
    
    if (m->owner == UINT8_MAX) {
        m->owner = task_running;
    }
    
    else if (m->owner != task_running) {
    #if PRIORITY_INHERITANCE == ON
        if (ReadyQueue.tasks[task_running].task_prior >
            ReadyQueue.tasks[m->owner].task_prior) {

            ReadyQueue.tasks[m->owner].task_prior =
                    ReadyQueue.tasks[task_running].task_prior;
        }
    #endif

        m->queue[m->queue_input] = task_running;
        m->queue_input = __builtin_modsd(m->queue_input + 1, MAX_TASKS);
        m->queue_count++;
        ReadyQueue.tasks[task_running].task_state = BLOCKED;
        CONTEXT_SWITCH();
    }
    
    __builtin_enable_interrupts();
}

void mutex_unlock(mutex_t *m)
{
    __builtin_disable_interrupts();
    
    if (m->owner == task_running) {
        if (m->queue_count == 0) {
            ReadyQueue.tasks[m->owner].task_prior = 
                   ReadyQueue.tasks[m->owner].task_base_prior;
            m->owner = UINT8_MAX;
        }
        
        else {
            uint8_t proxima_tarefa = m->queue[m->queue_output];
            ReadyQueue.tasks[m->owner].task_prior = 
                    ReadyQueue.tasks[m->owner].task_base_prior;
            m->owner = proxima_tarefa;
            ReadyQueue.tasks[proxima_tarefa].task_state = READY;
            m->queue_output = __builtin_modsd(m->queue_output+1, MAX_TASKS);
            m->queue_count--;
            
            #if PRIORITY_INHERITANCE == ON
            uint8_t pos = m->queue_output;

            for (uint8_t i = 0; i < m->queue_count; i++) {
                uint8_t tarefa_aguardando = m->queue[pos];
                pos = __builtin_modsd(pos + 1, MAX_TASKS);

                if (ReadyQueue.tasks[tarefa_aguardando].task_prior >
                    ReadyQueue.tasks[m->owner].task_prior) {

                    ReadyQueue.tasks[m->owner].task_prior =
                            ReadyQueue.tasks[tarefa_aguardando].task_prior;
                }
            }
        #endif
       }   
    }
    __builtin_enable_interrupts();
}
