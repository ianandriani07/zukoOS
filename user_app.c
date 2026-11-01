#include <xc.h>
#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "message.h"
#include "uart.h"
#include "pwm.h"


m_queue_t fila_m;
mutex_t mutex_teste;

TASK tarefa_a(void)
{
    //char letras[4] = {'a', 'b', 'c', 'd'};
    //int index = 0;
    
    while (1) {
        mutex_lock(&mutex_teste);
        LATEbits.LATE0 = 1;
        task_delay(250);
        LATEbits.LATE0 = 0;
        mutex_unlock(&mutex_teste);
        task_delay(250);      
    }
}

TASK tarefa_b(void)
{
    pwm_driver_t pwm;
   
    pwm_config(&pwm, 1000);
    pwm_start(&pwm);
    
    pwm_set_duty(&pwm, 100);
    
   
    while (1) {
        LATEbits.LATE2 ^= 1;
        mutex_lock(&mutex_teste);
        mutex_lock(&mutex_teste);
        LATEbits.LATE1 = 1;
        task_delay(100);
        LATEbits.LATE1 = 0;
        mutex_unlock(&mutex_teste);
        task_delay(100);
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
    m_queue_init(&fila_m);
    mutex_init(&mutex_teste);
    UART_init(9600);
}
