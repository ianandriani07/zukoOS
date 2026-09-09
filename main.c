#include "kernel.h"
#include "user_app.h"


int main()
{
    // Cria as estruturas de dados do SO
    os_config();
    
    // Cria as tarefas de usuário
    create_task(tarefa_a, 5);
    create_task(tarefa_b, 5);
    create_task(tarefa_c, 5);
    
    // Inicializa o escalonador
    os_start_scheduler();
    
    while (1);
    
    return 0;
}
