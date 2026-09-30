#ifndef MEM_H
#define	MEM_H

#include <stdint.h>

// Funções
void os_heap_init();
void *os_malloc(uint16_t nr_bytes);
void os_free(void *address);


#endif	/* MEM_H */

