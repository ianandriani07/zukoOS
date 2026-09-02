#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"


extern uint16_t *task_running_tos;


void create_task(callback task, uint8_t prior);
void init_stack(tcb_t *task);

#define SAVE_CONTEXT() \
do { \
  asm volatile ( \
    "PUSH W0 \n" \
    "PUSH W1 \n" \
    "PUSH W2 \n" \
    "PUSH W3 \n" \
    "PUSH W4 \n" \
    "PUSH W5 \n" \
    "PUSH W6 \n" \
    "PUSH W7 \n" \
    "PUSH W8 \n" \
    "PUSH W9 \n" \
    "PUSH W10 \n" \
    "PUSH W11 \n" \
    "PUSH W12 \n" \
    "PUSH W13 \n" \
    "PUSH W14 \n" \
    "MOV RCOUNT, W0 \n" \
    "PUSH W0 \n" \
    "MOV TBLPAG, W0 \n" \
    "PUSH W0 \n" \
    "MOV PSVPAG, W0 \n" \
    "PUSH W0 \n" \
    "MOV CORCON, W0 \n" \
    "PUSH W0 \n" \
    "MOV W15, _task_running_tos \n" \
  ) \
} while(0);

#define RESTORE_CONTEXT() \
do { \
  asm volatile ( \
    "MOV _task_running_tos, W15 \n" \
    "POP W0 \n" \
    "MOV W0, CORCON \n" \
    "POP W0 \n" \
    "MOV W0, PSVPAG \n" \
    "POP W0 \n" \
    "MOV W0, TBLPAG \n" \
    "POP W0 \n" \
    "MOV W0, RCOUNT \n" \
    "POP W14 \n" \
    "POP W13 \n" \
    "POP W12 \n" \
    "POP W11 \n" \
    "POP W10 \n" \
    "POP W9 \n" \
    "POP W8 \n" \
    "POP W7 \n" \
    "POP W6 \n" \
    "POP W5 \n" \
    "POP W4 \n" \
    "POP W3 \n" \
    "POP W2 \n" \
    "POP W1 \n" \
    "POP W0 \n" \
    "RETFIE \n" \
  ) \
} while(0);

#endif	/* KERNEL_H */

