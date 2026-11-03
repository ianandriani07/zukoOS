#include "kernel.h"
#include "user_app.h"

int main()
{
    // Cria as estruturas de dados do SO
    os_config();
    
    create_task(task_scram, 4);
    create_task(task_read_temp, 3);
    create_task(task_control, 3);
    create_task(task_alarm, 3);
    create_task(task_diagA, 2);
    create_task(task_diagB, 2);
    create_task(task_panel, 1);
    
    // Inicializa o escalonador
    os_start_scheduler();
    
    while (1);
    
    return 0;
}
