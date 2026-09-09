#include "user_app.h"
#include <xc.h>

TASK tarefa_a(void)
{
    while (1) {
        LATEbits.LATE0 ^= 1;
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
    while (1) {
        LATEbits.LATE2 ^= 1;
    }
}

void config_user_app(void)
{
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;    
}


