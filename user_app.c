#include "user_app.h"
#include "kernel.h"
#include <xc.h>
#include "sync.h"
#include "message.h"


sem_t semaforo;
m_queue_t fila_m;


TASK tarefa_a(void)
{
    char letras[4] = {'a', 'b', 'c', 'd'};
    int index = 0;
    
    while (1) {
        LATEbits.LATE0 ^= 1;
        //sem_wait(&semaforo);
        //task_delay(10);
        
        m_queue_write(&fila_m, letras[index]);
        index++;
        if (index == 4) index = 0;
        
    }
}
TASK tarefa_b(void)
{
    while (1) {
        LATEbits.LATE1 ^= 1;
        task_delay(100);
        //sem_post(&semaforo);
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
    //sem_init(&semaforo, 10);
    m_queue_init(&fila_m);
}


