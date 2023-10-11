#include "philosophers.h"

static int     the_beginning_of_life(t_table *table, t_philo *philo);

/*

char    *pos_int_to_stack_str(int nb, char *buf, int bufsize)
{
    int i;
    int place;
    
    i = bufsize - 1;
    buf[i--] = '\0';
    if (nb == 0)
    {
        buf[i] = '0';
        return (&buf[i]);
    }
    while (nb)
    {
        place = (nb % 10) + '0';
        buf[i--] = place;
        nb /= 10;
    }
    return (&buf[++i]);
} */

void    *monitor_death_or_full(void *mytable)
{
    int     i;
    int     save_must_exit;
    t_table *table;
    t_philo *philo;
    
    table = (t_table *)mytable;
    philo = &table->philo;
    while (1)
    {
        sem_wait(table->check_death);
        if (!(philo->last_meal_start == 0 \
        || philo->meals_i_had == table->max_meals))
        {
            table->cur_time = milisec_epoch();
            if (table->cur_time - philo->last_meal_start >= table->to_die)
            {
                philo->died = 1;
                broadcast_life_state(table, PRINT_DEATH, table->cur_time - table->open_time);
                sem_post(table->exit_signal);
                return (NULL);
            }
        }
        if (philo->meals_i_had == table->max_meals)
        {
            philo->died = 1;
            sem_post(table->all_meals);
            sem_post(table->check_death);
            return (NULL);
        }
        sem_post(table->check_death);
        usleep(TACTICAL_WAIT);
    }
    return (NULL);
}


/*
int     philo_death_semaphore_and_thread(t_table *table, t_philo *philo)
{
    char    *start;
    char    str_id[11];
    
    start = pos_int_to_stack_str(philo->my_id, str_id, sizeof(str_id));
    philo->mydeath_name = ft_strjoin("mydeath_", start);
    if (!philo->mydeath_name)
        return (0);
    philo->mydeath = sem_open(philo->mydeath_name, O_CREAT, 0644, 1);
    if (pthread_create(&philo->monitor, NULL, monitor_death_or_full, table))
        clean_table(table, false, EXIT_FAILURE);
    pthread_detach(philo->monitor);
    return (1);
}*/




void    the_life_of_a_philosopher(t_table *table, t_philo *philo)
{
    if (!the_beginning_of_life(table, philo))
        return ;
    if (table->num_seats == 1)
        return (the_life_of_a_lonely_philo(table, philo));
    while (1)
    {
        if (!take_first_fork(table, philo) \
        || !take_second_fork(table, philo) \
        || !time_to_eat(table, philo) \
        || !time_to_sleep(table, philo) \
        || !time_to_think(table, philo))
            break ;
    }
    //printf("philo %d exit loop\n", philo->my_id);

    //pthread_detach(table->philo.monitor);
    //pthread_detach(table->philo.wait_signal);

    pthread_join(table->philo.self_monitor, NULL);
    clean_table(table, false, EXIT_SUCCESS);
}

void    the_life_of_a_lonely_philo(t_table *table, t_philo *philo)
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
        return ;
    }
    sem_wait(table->check_death);
    if (!philo->died)
    {
        broadcast_life_state(table, PRINT_FORK, 0);
        sem_post(table->check_death);
        philo_sleep(table, philo, philo->last_meal_start + table->to_die);
    }
    else
        sem_post(table->check_death);
    sem_post(table->forks);
}

static int     the_beginning_of_life(t_table *table, t_philo *philo)
{
    sem_wait(table->start_execution);
    sem_post(table->start_execution);
    
    if (pthread_create(&philo->self_monitor, NULL, monitor_death_or_full, table))
        clean_table(table, false, EXIT_FAILURE);
    //pthread_detach(philo->monitor);
    
    sem_wait(table->check_death);
    philo->last_meal_start = milisec_epoch();
    sem_post(table->check_death);
    return (1);
}
