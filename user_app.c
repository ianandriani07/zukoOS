#include <xc.h>
#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "message.h"
#include "uart.h"
#include "pwm.h"
#include "uart.h"
#include "adc.h"

typedef enum {
    FINE = 0,
    NOT_FINE = 1,
    SCRAM = 2
} state_t;

static const char *state_name[] = { "NORMAL", "ALERTA", "SCRAM" };

volatile state_t state = FINE;
volatile uint8_t temperature = 0;

sem_t scram_sem;
mutex_t state_mutex;
mutex_t uart_mutex;

m_queue_t temp_queue;

pwm_driver_t pump_driver;

TASK task_scram() 
{
    sem_wait(&scram_sem);
    LATEbits.LATE2 = 1; 
    
    pwm_set_duty(&pump_driver, 100);
    
    mutex_lock(&state_mutex);
    state = SCRAM;
    mutex_unlock(&state_mutex);
    
    mutex_lock(&uart_mutex);
    LATEbits.LATE3 = 1;
    UART_write_new_line("SCRAM: desligamento de emergencia iniciado... Fodeu!");
    mutex_unlock(&uart_mutex);

    while (1) task_delay(250);
}

TASK task_read_temp()
{
    while (1) {
        uint16_t raw = adc_read();
        char downcast = (char)(raw >> 2); // This should change almost nothing
        m_queue_write(&temp_queue, downcast);
        task_delay(25); // 100ms
    }
}

void config_user_app(void)
{   
    sem_init(scram_sem, 0);
    mutex_init(&state_mutex);
    mutex_init(&uart_mutex);
    
    pwm_config(&pump_driver, FREQ);
    pwm_start(&pump_driver);
    
    adc_init();
    
    m_queue_init(&temp_queue, 8);
    
    UART_init(9600);
}
