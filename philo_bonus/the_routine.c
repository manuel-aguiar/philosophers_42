#include "philosophers.h"

int take_first_fork(t_table *table, t_philo *philo)
{
    sem_wait(table->check_death);
    if (!philo->died)
    {
        sem_post(table->check_death);
        sem_wait(table->forks);
    }
    else
    {
        sem_post(table->check_death);
        return (0);
    }
    sem_wait(table->check_death);    
    if (!philo->died)
        broadcast_life_state(table, PRINT_FORK, 0);
    else
    {
        sem_post(table->forks);
        sem_post(table->check_death);
        return (0);
    }
    sem_post(table->check_death); 
    return (1);
}

static int got_the_second_one(t_table *table, t_philo *philo)
{
    sem_wait(table->check_death);
    if (!philo->died)
    {
        broadcast_life_state(table, PRINT_FORK, 0);
        philo->last_meal_start = philo->cur_time;
        sem_post(table->check_death);
    }
    else
    {
        sem_post(table->forks);
        sem_post(table->forks);
        sem_post(table->check_death);
        return (0);
    }
    return (1);
}

int take_second_fork(t_table *table, t_philo *philo)
{
    sem_wait(table->check_death);
    if (!philo->died)
    {
        sem_post(table->check_death);
        sem_wait(table->forks);
    }
    else
    {
        sem_post(table->forks);
        sem_post(table->check_death);
        return (0);
    }
    return (got_the_second_one(table, philo));
}
