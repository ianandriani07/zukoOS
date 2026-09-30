#include <xc.h>
#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "message.h"
#include "io.h"


m_queue_t fila_m;

TASK tarefa_a(void)
{
    char letras[4] = {'a', 'b', 'c', 'd'};
    int index = 0;
    
    while (1) {
        LATEbits.LATE0 ^= 1;
        m_queue_write(&fila_m, letras[index]);
        index++;
        if (index == 4) index = 0;        
    }
}

TASK tarefa_b(void)
{
    while (1) {
        LATEbits.LATE1 ^= 1;
    }
}

TASK tarefa_c(void)
{
    char letra;
    
    while (1) {
        LATEbits.LATE2 ^= 1;        
        m_queue_read(&fila_m, &letra);
    }
}

void config_user_app(void)
{
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISFbits.TRISF3 = 0;
    m_queue_init(&fila_m);
}
