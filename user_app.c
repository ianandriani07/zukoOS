#include <xc.h>
#include <stdbool.h>
#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "message.h"
#include "uart.h"
#include "pwm.h"
#include "uart.h"
#include "adc.h"

#define NOT_FINE_THRESHOLD   150  // ~1,94 V (by Claude)
#define CRITICAL_THRESHOLD   210  // ~2,72 V (by Claude)
#define N_TEST               20000u

typedef enum {
    FINE = 0,
    NOT_FINE = 1,
    SCRAM = 2
} reactor_state_t;

static const char *state_name[] = { "NORMAL", "ALERTA", "SCRAM" };

volatile reactor_state_t state = FINE;
volatile uint8_t temperature = 0;

sem_t scram_sem;
#if DEMO_HERANCA == ON
static sem_t demo_sem;

extern queue_t ReadyQueue;
extern uint8_t task_running;
#endif
mutex_t state_mutex;
mutex_t uart_mutex;

m_queue_t temp_queue;

pwm_driver_t pump_driver;

TASK task_scram(void) 
{
    sem_wait(&scram_sem);
    LATEbits.LATE2 = 1; 
    
    pwm_set_duty(&pump_driver, 99);
    
    mutex_lock(&state_mutex);
    state = SCRAM;
    mutex_unlock(&state_mutex);
    
    mutex_lock(&uart_mutex);
    LATEbits.LATE3 = 1;
    UART_write_new_line("*** SCRAM: desligamento de emergencia iniciado... Fodeu! ***");
    mutex_unlock(&uart_mutex);

    while (1) task_delay(250);
}

TASK task_read_temp(void)
{
    while (1) {
        uint16_t raw = adc_read();
        char downcast = (char)(raw >> 2); // This should change almost nothing
        m_queue_write(&temp_queue, downcast);
        task_delay(25); // ~100ms
    }
}

TASK task_control(void)
{
    char msg;
    bool we_fucked = false;

    while (1) {
        m_queue_read(&temp_queue, &msg);
        
        uint8_t t = (uint8_t)msg;
        uint8_t button_is_pressed = (PORTDbits.RD6 == 0);

        mutex_lock(&state_mutex);
        temperature = t;
        if (!we_fucked)
            state = (t >= NOT_FINE_THRESHOLD) ? NOT_FINE : FINE;
        mutex_unlock(&state_mutex);

        if (we_fucked) continue;

        if (t >= CRITICAL_THRESHOLD || button_is_pressed) {
            we_fucked = true;

        #if DEMO_HERANCA == ON
            sem_post(&demo_sem);
        #else
            sem_post(&scram_sem);
        #endif

            continue;
        }

        pwm_set_duty(&pump_driver, 20 + (t * 80u) / 255u);
    }
}

TASK task_alarm(void)
{
    while (1) {
        mutex_lock(&state_mutex);
        reactor_state_t local_state = state;
        mutex_unlock(&state_mutex);

        if (local_state == SCRAM) {
            // Light is always on.
            LATEbits.LATE4 = 1;
            task_delay(25);
        } else {
            // Light blinks
            LATEbits.LATE4 ^= 1;
            task_delay(local_state == NOT_FINE ? 25 : 125);   // 100ms / 500ms
        }
    }
}

TASK task_diagA(void)
{
    while (1) {
        for (volatile uint16_t i = 0; i < N_TEST; i++) {
            LATEbits.LATE0 ^= 1;
        }
        task_delay(10);
    }
}

TASK task_diagB(void)
{
    while (1) {
        for (volatile uint16_t i = 0; i < N_TEST; i++) {
            LATEbits.LATE1 ^= 1;
        }
        task_delay(10);
    }
}

#if DEMO_HERANCA == ON
static void demo_heranca(void)
{
    uint8_t painel = task_running;
    volatile tcb_t *tcb = &ReadyQueue.tasks[painel];

    uint8_t antes;
    uint8_t durante;
    uint8_t depois;
    uint8_t esperando;
    uint8_t dono_correto;

    mutex_lock(&uart_mutex);
    LATEbits.LATE5 = 1;

    UART_write_new_line(
        "[PI] Painel com mutex. Aperte o botao de emergencia."
    );

    /* Espera o botao sem liberar o mutex da UART. */
    sem_wait(&demo_sem);

    antes = tcb->task_prior;

    /*
     * SCRAM tem prioridade 4: acorda e tenta obter
     * o mutex que continua pertencendo ao painel.
     */
    sem_post(&scram_sem);

    /* Mesma transmissao pendente nos testes ON e OFF. */
    UART_write_new_line(
        "[PI] Transmissao fixa do painel: "
        "01234567890123456789012345678901234567890123456789"
    );

    /* Guarda as evidencias antes de liberar o mutex. */
    durante = tcb->task_prior;
    esperando = uart_mutex.queue_count;
    dono_correto = (uart_mutex.owner == painel);

    LATEbits.LATE5 = 0;
    mutex_unlock(&uart_mutex);

    depois = tcb->task_prior;

    /*
     * Imprime somente depois da disputa.
     * Este lock tambem impede misturar com a mensagem SCRAM.
     */
    mutex_lock(&uart_mutex);

#if PRIORITY_INHERITANCE == ON
    UART_write_string("[PI] ON | painel: ");
#else
    UART_write_string("[PI] OFF | painel: ");
#endif

    UART_write_int(antes);
    UART_write_string(" -> ");
    UART_write_int(durante);
    UART_write_string(" -> ");
    UART_write_int(depois);

    UART_write_string(" | esperando: ");
    UART_write_int(esperando);

    UART_write_string(" | dono era painel: ");
    UART_write_int(dono_correto);
    UART_write_new_line("");

    mutex_unlock(&uart_mutex);
}
#endif

TASK task_panel(void)
{
    #if DEMO_HERANCA == ON
        demo_heranca();
    #endif


        /* Mantenha todo o conteudo atual deste while. */
    while (1) {
        mutex_lock(&state_mutex);
        uint8_t t = temperature;
        reactor_state_t s = state;
        mutex_unlock(&state_mutex);

        mutex_lock(&uart_mutex);
        LATEbits.LATE5 = 1;
        UART_write_string("[ZukoOS] temperatura = ");
        UART_write_int(t * 4);
        UART_write_string(" ºC | estado = ");
        UART_write_new_line(state_name[s]);
        UART_write_new_line("[ZukoOS] bomba e sensores verificados, aguardando proximo ciclo...");
        LATEbits.LATE5 = 0;
        mutex_unlock(&uart_mutex);

        task_delay(250);
    }
}

void config_user_app(void)
{
    TRISE = 0xC0;
    LATE  = 0xC0;
    TRISDbits.TRISD6 = 1;
    
    sem_init(&scram_sem, 0);
    #if DEMO_HERANCA == ON
        sem_init(&demo_sem, 0);
    #endif
    mutex_init(&state_mutex);
    mutex_init(&uart_mutex);
    
    pwm_config(&pump_driver, FREQ);
    pwm_start(&pump_driver);
    pwm_set_duty(&pump_driver, 20);
    
    adc_init();
    
    m_queue_init(&temp_queue, 8);
    
    UART_init(9600);
}
