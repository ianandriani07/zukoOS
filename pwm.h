#ifndef PWM_H
#define	PWM_H

#ifndef FCY
#define FCY 4000000
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct pwm_driver {
    uint16_t periodo;
    uint16_t ciclo;
    bool status;
} pwm_driver_t; 

void pwm_config(pwm_driver_t *pwm, uint16_t frequencia);
void pwm_start(pwm_driver_t *pwm);
void pwm_stop(pwm_driver_t *pwm);
void pwm_set_duty(pwm_driver_t *pwm, uint16_t duty);


#endif	/* PWM_H */

