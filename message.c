#include <builtins.h>
#include <stdio.h>

#include "message.h"
#include "sync.h"
#include "mem.h"

int m_queue_init(m_queue_t *queue, uint8_t size)
{
    if (size == 0) return -1;
#if DYNAMIC_ALLOCATION == ON
    queue->size = size;
    queue->queue = (char*)os_malloc(sizeof(char) * ((size + 1) & ~1u));
    if (queue->queue == NULL) return -1;
#else
    if (size > SIZE_MESSAGE_QUEUE) return -1;
    queue->size = SIZE_MESSAGE_QUEUE;
#endif
    queue->pos_read     = 0;
    queue->pos_write    = 0;
    sem_init(&queue->s_write, queue->size);
    sem_init(&queue->s_read, 0);
    
    return 0;
}

void m_queue_write(m_queue_t *queue, char msg)
{
    sem_wait(&queue->s_write);
    queue->queue[queue->pos_write] = msg;
    queue->pos_write = __builtin_modsd(queue->pos_write+1, queue->size);
    //queue->pos_write = (queue->pos_write+1) % SIZE_MESSAGE_QUEUE;
    sem_post(&queue->s_read);
}

void m_queue_read(m_queue_t *queue, char *msg)
{
    sem_wait(&queue->s_read);
    *msg = queue->queue[queue->pos_read];
    queue->pos_read = __builtin_modsd(queue->pos_read+1, queue->size);
    //queue->pos_read = (queue->pos_read+1) % SIZE_MESSAGE_QUEUE;
    sem_post(&queue->s_write);   
}

