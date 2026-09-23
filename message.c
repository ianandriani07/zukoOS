#include <builtins.h>

#include "message.h"
#include "sync.h"

void m_queue_init(m_queue_t *queue)
{
    queue->pos_read     = 0;
    queue->pos_write    = 0;
    sem_init(&queue->s_write, SIZE_MESSAGE_QUEUE);
    sem_init(&queue->s_read, 0);
}

void m_queue_write(m_queue_t *queue, char msg)
{
    __builtin_disable_interrupts();
    
    sem_wait(&queue->s_write);
    queue->queue[queue->pos_write] = msg;
    queue->pos_write = (queue->pos_write+1) % SIZE_MESSAGE_QUEUE;
    sem_post(&queue->s_read);    
    
    __builtin_enable_interrupts();
}

void m_queue_read(m_queue_t *queue, char *msg)
{
    __builtin_disable_interrupts();
    
    sem_wait(&queue->s_read);
    *msg = queue->queue[queue->pos_read];
    queue->pos_read = (queue->pos_read+1) % SIZE_MESSAGE_QUEUE;
    sem_post(&queue->s_write);
    
    __builtin_enable_interrupts();    
}

