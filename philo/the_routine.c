#include "philosophers.h"

int take_first_fork(t_table *table, t_philo *philo)
{
    pthread_mutex_lock(&table->check_death);
    if (!table->exit_table)
    {
        pthread_mutex_unlock(&table->check_death);
        pthread_mutex_lock(philo->first_fork);
    }
    else
    {
        pthread_mutex_unlock(&table->check_death);
        return (0);
    }
    pthread_mutex_lock(&table->check_death);    
    if (!table->exit_table)
        broadcast_life_state(table, philo, PRINT_FORK, 0);
    else
    {
        pthread_mutex_unlock(philo->first_fork);
        pthread_mutex_unlock(&table->check_death);
        return (0);
    }
    pthread_mutex_unlock(&table->check_death); 
    return (1);
}

static int got_the_second_one(t_table *table, t_philo *philo)
{
    pthread_mutex_lock(&table->check_death);
    if (!table->exit_table)
    {
        broadcast_life_state(table, philo, PRINT_FORK, 0);
        philo->last_meal_start = philo->cur_time;
        pthread_mutex_unlock(&table->check_death);
    }
    else
    {
        pthread_mutex_unlock(philo->second_fork);
        pthread_mutex_unlock(philo->first_fork);
        pthread_mutex_unlock(&table->check_death);
        return (0);
    }
    return (1);
}

int take_second_fork(t_table *table, t_philo *philo)
{
    pthread_mutex_lock(&table->check_death);
    if (!table->exit_table)
    {
        pthread_mutex_unlock(&table->check_death);
        pthread_mutex_lock(philo->second_fork);
    }
    else
    {
        pthread_mutex_unlock(philo->first_fork);
        pthread_mutex_unlock(&table->check_death);
        return (0);
    }
    return (got_the_second_one(table, philo));
}
