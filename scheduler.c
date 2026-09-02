#include "scheduler.h"
#include "os_config.h"

uint8_t scheduler()
{
#if DEFAULT_SCHEDULER == RR_SCHEDULER
    return rr_scheduler();
#else
    return prior_scheduler();
#endif    
}

uint8_t rr_scheduler()
{
    
}

uint8_t prior_scheduler()
{
    
}

