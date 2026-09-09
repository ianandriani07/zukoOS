#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>

void config_timer_0(void);
void __attribute__ ((interrupt, no_auto_psv, naked)) _T1Interrupt(); 


#endif	/* TIMER_H */

