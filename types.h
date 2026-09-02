#ifndef TYPES_H
#define	TYPES_H

#include "os_config.h"
#include <stdint.h>

typedef void (*callback)(void);

typedef void TASK;

typedef enum {READY = 0, RUNNING, WAITING} state_t;

typedef struct tcb {
    uint8_t task_id;
    uint8_t task_prior;
    callback task_func;
    state_t task_state;
    uint16_t task_stack[MAX_STACK_SIZE];
    uint16_t *sp;
} tcb_t;

typedef struct queue {
    tcb_t tasks[MAX_TASKS];
    uint8_t queue_size;
} queue_t;

#endif	/* TYPES_H */

