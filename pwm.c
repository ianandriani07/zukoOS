#include <p24FJ128GA010.h>

#include "pwm.h"

void pwm_config(pwm_driver_t *pwm, uint16_t frequencia)
{
    if (frequencia == 0) return;
    
    pwm->periodo    = 0;
    pwm->ciclo      = 0;
    pwm->status     = false;
    
    // Configuração de hardware
    OC1CON            = 0;    
    T2CON             = 0;
    OC1CONbits.OCTSEL = 0;      // Timer 2
    OC1CONbits.OCM    = 0b000;  // Captura compare disable
    
    T2CONbits.TCKPS   = 0;      // Preescaler 1:1

    // Cálculo do período (PR2 = (Fcy / fPwm)-1)
    pwm->periodo      = (FCY / frequencia) -1;
    
    OC1R    = 0;
    OC1RS   = 0;
    TMR2    = 0;
}

void pwm_start(pwm_driver_t *pwm)
{
    if (pwm->status) return;
    
    OC1CONbits.OCM  = 0b000;
    T2CONbits.TON   = 0;

    TMR2 = 0;
    PR2  = pwm->periodo;   
        
    pwm->status = true;
    
    OC1R        = pwm->ciclo;
    OC1RS       = pwm->ciclo;
    PR2         = pwm->periodo;    
    
    OC1CONbits.OCM   = 0b110;    
    T2CONbits.TON    = 1;
}

void pwm_stop(pwm_driver_t *pwm)
{
    pwm->status = false;
    
    OC1CONbits.OCM  = 0b000;
    T2CONbits.TON   = 0;    
}

void pwm_set_duty(pwm_driver_t *pwm, uint16_t duty)
{
    uint32_t ciclo;
    
    if (!pwm->status) return;
    
    if (duty > 100) duty = 100;
    
    ciclo = ((uint32_t)(pwm->periodo + 1UL) * duty) / 100UL;

    if (ciclo > pwm->periodo) ciclo = pwm->periodo;

    pwm->ciclo = (uint16_t)ciclo;
    
    OC1RS       = pwm->ciclo;
}

