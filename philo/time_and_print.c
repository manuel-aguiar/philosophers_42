#include "philosophers.h"

time_t milisec_epoch(void)
{
    struct timeval   cur;
    
    if (gettimeofday(&cur, NULL) == -1)
        return (0);
    return (cur.tv_sec * 1000 + cur.tv_usec / 1000);
}

int broadcast_life_state(t_table *table, t_philo *philo, \
                        char *state, time_t time_elapsed)
{
    if (!time_elapsed)
    {
        philo->cur_time = milisec_epoch();
        time_elapsed = philo->cur_time - table->open_time;
    }
    printf("%-10ld %-5d %s\n", time_elapsed, philo->my_id, state);
    return (1);
}