#include <xc.h>
#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "message.h"
#include "uart.h"
#include "pwm.h"


m_queue_t fila_m;
mutex_t mutex_teste;
extern queue_t ReadyQueue;
volatile uint8_t b_recebeu_mutex = 0;

TASK tarefa_a(void)
{
    mutex_lock(&mutex_teste);
    LATEbits.LATE0 = 1;

    for (volatile uint32_t i = 0; i < 2000000UL; i++) {
        Nop();
    }

    LATEbits.LATE0 = 0;
    mutex_unlock(&mutex_teste);

    while (1) {
        task_delay(100);
    }
}

TASK tarefa_b(void)
{
    task_delay(50);

    mutex_lock(&mutex_teste);

    b_recebeu_mutex = 1;

    if (ReadyQueue.tasks[2].task_prior == 3 &&
        mutex_teste.queue_count > 0 &&
        mutex_teste.queue[mutex_teste.queue_output] == 3) {
        LATEbits.LATE1 = 1;
    }

    mutex_unlock(&mutex_teste);

    while (1) {
        task_delay(100);
    }
}

TASK tarefa_c(void)
{
    task_delay(100);

    mutex_lock(&mutex_teste);

    if (b_recebeu_mutex == 1) {
        LATEbits.LATE2 = 1;
    }

    mutex_unlock(&mutex_teste);

    while (1) {
        task_delay(100);
    }
}

void config_user_app(void)
{
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISFbits.TRISF3 = 0;
    m_queue_init(&fila_m, 32);
    mutex_init(&mutex_teste);
    UART_init(9600);
}
