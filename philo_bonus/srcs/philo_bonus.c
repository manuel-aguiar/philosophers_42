/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include "philosophers.h"


int open_hell_s_kitchen(t_table *table)
{
    int i;
    
    if (table->max_meals == 0)
        return (1);
    sem_wait(table->start_execution);
    table->open_time = milisec_epoch();
    if (!prepare_forks_and_ids(table))
    {
        i = 0;
        while (i++ < table->last_good_philo)
            sem_post(table->exit_signal);
        printf("philo: fork: last good philo %d\n", table->last_good_philo);
        sem_post(table->start_execution);
        return (0);
    }
    sem_post(table->start_execution);
    return (1);
}

int main(int ac, char **av)
{
    t_table    table;
    
    if (ac < 5 || ac > 6)
    {
        printf("philo: invalid arguments\n");
        return (0);
    }
    if (!prepare_table(&table, ac, av))
    {
        printf("philo: syscall failed\n");
        clean_table(&table, true, EXIT_FAILURE);
        return (0);
    }
    //printf("hey man\n");
    open_hell_s_kitchen(&table);
    
    
    clean_table(&table, true, EXIT_SUCCESS);
    return (0);
}
