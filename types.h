#ifndef TYPES_H
#define	TYPES_H

#include "os_config.h"
#include <stdint.h>

//#define IDLE 1

typedef void (*callback)(void);

typedef void TASK;

typedef enum {READY = 0, RUNNING, WAITING, BLOCKED} state_t;

typedef struct tcb {
    uint8_t task_id;
    uint8_t task_prior;
    uint16_t task_delay;
    callback task_func;
    state_t task_state;
    uint16_t task_stack[MAX_STACK_SIZE];
    uint16_t *sp;
} tcb_t;

typedef struct queue {
    tcb_t tasks[MAX_TASKS+1];
    uint8_t queue_size;
} queue_t;

typedef struct sem
{
    uint8_t sem_queue[MAX_TASKS];
    int sem_count;
    uint8_t s_queue_input;
    uint8_t s_queue_output;
} sem_t;

#endif	/* TYPES_H */

