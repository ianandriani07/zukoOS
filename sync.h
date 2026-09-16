#ifndef SYNC_H
#define	SYNC_H

#include "types.h"

void sem_init(sem_t *s, uint8_t value);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);


#endif	/* SYNC_H */

