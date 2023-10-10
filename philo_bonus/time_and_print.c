#include "philosophers.h"

time_t milisec_epoch(void)
{
    struct timeval   cur;
    
    if (gettimeofday(&cur, NULL) == -1)
        return (0);
    return (cur.tv_sec * 1000 + cur.tv_usec / 1000);
}

int broadcast_life_state(t_table *table, \
                        char *state, time_t time_elapsed)
{
    if (!time_elapsed)
    {
        table->philo.cur_time = milisec_epoch();
        time_elapsed = table->philo.cur_time - table->open_time;
    }
    //sem_wait(table->damn_print);
    if (!table->philo.died)
        printf("%-10ld %-5d %s\n", time_elapsed, table->philo.my_id, state);
    //sem_post(table->damn_print);
    return (1);
}
