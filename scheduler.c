#include <builtins.h>
#include <stdbool.h>

#include "scheduler.h"
#include "os_config.h"
#include "types.h"
#include "kernel.h"

extern queue_t ReadyQueue;
extern uint8_t task_running;
extern uint16_t *task_running_tos;


uint8_t scheduler()
{
#if DEFAULT_SCHEDULER == RR_SCHEDULER
    return rr_scheduler();
#else
    return prior_scheduler();
#endif    
}

uint8_t rr_scheduler()
{
    uint8_t next_task = task_running, idle_count = 0;
    
    do {
        next_task = __builtin_modsd(next_task+1, ReadyQueue.queue_size);
        
        if (ReadyQueue.tasks[next_task].task_func == idle) {
            idle_count++;
            if (idle_count >= 2) {
                return 0;
            }
        }
        
    } while (ReadyQueue.tasks[next_task].task_state != READY ||
             ReadyQueue.tasks[next_task].task_func == idle);
    
    return next_task;
}

void sort_tasks() {
    int n = ReadyQueue.queue_size;
    bool swapped;
  
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            uint8_t before_idx = ReadyQueue.ordered_tasks[j];
            uint8_t after_idx = ReadyQueue.ordered_tasks[j + 1];
            
            uint8_t before_priority = ReadyQueue.tasks[before_idx].task_prior;
            uint8_t after_priority = ReadyQueue.tasks[after_idx].task_prior;
            
            if (before_priority < after_priority) {
                ReadyQueue.ordered_tasks[j] = after_idx;
                ReadyQueue.ordered_tasks[j + 1] = before_idx;
                swapped = true;
            }
        }
      
        if (!swapped)
            break;
    }
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

uint8_t last_queue_size = 0;

uint8_t prior_scheduler()
{
    // If the list changes sizes (dynamic task) it should still work.
    if (last_queue_size != ReadyQueue.queue_size) {
        sort_tasks();
        last_queue_size = ReadyQueue.queue_size;
    }
    
    uint8_t idx_last_task = 0;
    uint8_t highest_priority = 0;
    for (uint8_t i = 0; i < ReadyQueue.queue_size; i++) {
        uint8_t idx = ReadyQueue.ordered_tasks[i];
        if (idx == task_running) idx_last_task = i;
        
        if (ReadyQueue.tasks[idx].task_state == READY) 
            highest_priority = MAX(highest_priority, ReadyQueue.tasks[idx].task_prior);
    }
    
    for (uint8_t i = 0; i < ReadyQueue.queue_size; i++) {
          idx_last_task = __builtin_modsd(idx_last_task+1, ReadyQueue.queue_size);
          tcb_t *task = &ReadyQueue.tasks[ReadyQueue.ordered_tasks[idx_last_task]];

          if (task->task_state == READY && task->task_prior == highest_priority)
              return ReadyQueue.ordered_tasks[idx_last_task];
    }

    return 0;
}