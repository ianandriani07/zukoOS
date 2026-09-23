#ifndef MESSAGE_H
#define	MESSAGE_H

#include "types.h"

void m_queue_init(m_queue_t *queue);
void m_queue_write(m_queue_t *queue, char msg);
void m_queue_read(m_queue_t *queue, char *msg);


#endif	/* MESSAGE_H */

