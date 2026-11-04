#ifndef OS_CONFIG_H
#define	OS_CONFIG_H

#define FCY                 4000000UL
#define FREQ                1000UL // Ian said it was ok (?)

#define ON                  1
#define OFF                 0

#define MAX_TASKS           8
#define MAX_STACK_SIZE      128
#define SIZE_MESSAGE_QUEUE  4
#define HEAP_SIZE           1024

#define RR_SCHEDULER        1
#define PRIOR_SCHEDULER     2

#define DEFAULT_SCHEDULER PRIOR_SCHEDULER

#define PRIORITY_INHERITANCE OFF
#define DEMO_HERANCA        ON

#define DYNAMIC_ALLOCATION  ON

#endif	/* OS_CONFIG_H */

