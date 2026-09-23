#include "user_app.h"
#include "kernel.h"
#include <xc.h>
#include "sync.h"

sem_t semaforo;


TASK tarefa_a(void)
{
    while (1) {
        LATEbits.LATE0 ^= 1;
        sem_wait(&semaforo);
    }
}
TASK tarefa_b(void)
{
    while (1) {
        LATEbits.LATE1 ^= 1;
        task_delay(100);
        sem_post(&semaforo);
    }
}
TASK tarefa_c(void)
{
    while (1) {
        LATEbits.LATE2 ^= 1;
    }
}

void config_user_app(void)
{
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    sem_init(&semaforo, 0);
}


