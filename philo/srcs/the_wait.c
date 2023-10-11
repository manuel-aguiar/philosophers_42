#include "philosophers.h"

int philo_sleep(t_table *table, time_t end_sleep)
{
    while (1)
    {
        pthread_mutex_lock(&table->check_death);
        if (table->exit_table)
        {
            pthread_mutex_unlock(&table->check_death);
            return (0);
        }
        else
            pthread_mutex_unlock(&table->check_death);
        if (milisec_epoch() >= end_sleep)
            break ;
        usleep(TACTICAL_WAIT);
    }
    return (1);
}

int time_to_eat(t_table *table, t_philo *philo)
{
    int ret;
    
    pthread_mutex_lock(&table->check_death);
    if (!table->exit_table)
    {
        broadcast_life_state(table, philo, PRINT_EATING, 0);
        pthread_mutex_unlock(&table->check_death);
        if (!philo_sleep(table, milisec_epoch() + table->to_eat))
        {
            pthread_mutex_unlock(philo->second_fork); 
            pthread_mutex_unlock(philo->first_fork);
            return (0);
        }
    }
    else
        pthread_mutex_unlock(&table->check_death);
    pthread_mutex_lock(&table->check_death);
    table->finished_eating += (++philo->meals_i_had == table->max_meals) \
                            * (!table->exit_table);
    ret = (philo->meals_i_had == table->max_meals) + table->exit_table;
    pthread_mutex_unlock(philo->second_fork);
    pthread_mutex_unlock(philo->first_fork);
    pthread_mutex_unlock(&table->check_death);
    return (!ret);
}

int time_to_sleep(t_table *table, t_philo *philo)
{
    pthread_mutex_lock(&table->check_death);
    if (!table->exit_table)
    {
        broadcast_life_state(table, philo, PRINT_SLEEPING, 0);
        pthread_mutex_unlock(&table->check_death);
        if (!philo_sleep(table, milisec_epoch() + table->to_sleep))
            return (0);
        return (1);
    }
    else
    {
        pthread_mutex_unlock(&table->check_death);
        return (0);
    }
    return (1);    
}

int time_to_think(t_table *table, t_philo *philo)
{
    pthread_mutex_lock(&table->check_death);
    if (!table->exit_table)
    {
        broadcast_life_state(table, philo, PRINT_THINKING, 0);
        pthread_mutex_unlock(&table->check_death);
        return (1);
    }
    else
    {
        pthread_mutex_unlock(&table->check_death);
        return (0);
    }
    return (1);
}