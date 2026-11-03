#ifndef USER_APP_H
#define	USER_APP_H

#include "types.h"

TASK task_scram(void);
TASK task_read_temp(void);
TASK task_control(void);
TASK task_alarm(void);
TASK task_diagA(void);
TASK task_diagB(void);
TASK task_panel(void);

void config_user_app(void);


#endif	/* USER_APP_H */

