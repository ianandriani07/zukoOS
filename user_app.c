#include <xc.h>
#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "message.h"
#include "uart.h"
#include "pwm.h"


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
    pwm_driver_t pwm;
   
    pwm_config(&pwm, 1000);
    pwm_start(&pwm);
    
    pwm_set_duty(&pwm, 100);
    
   
    while (1) {
        LATEbits.LATE1 ^= 1;
        UART_write_new_line("Tarefa b executando ...");
    }
    
    pwm_stop(&pwm);
    
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
    //m_queue_init(&fila_m);
    UART_init(9600);
}
