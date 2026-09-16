#ifndef INT_H
#define	INT_H

#include <xc.h>

void config_int0();
void __attribute__ ((interrupt, no_auto_psv, naked)) _INT0Interrupt();

#endif	/* INT_H */

