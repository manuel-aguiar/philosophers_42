#include "philosophers.h"

int prepare_forks_and_ids(t_table *table)
{
    int i;
    
    i = 0;
    while (i < table->num_seats)
    {
        table->philo.my_id = i + 1;
        table->philo_pids[i] = fork();
        if (table->philo_pids[i] == -1)
        {
            table->last_good_philo = i;
            table->init_failed = true;
            return (0);
        }
        if (!table->philo_pids[i])
            the_life_of_a_philosopher(table, &table->philo);
        i++;
    }
    table->philo.my_id = 0;
    return (1);
}

int prepare_table(t_table *table, int ac, char **av)
{
    table->init_failed = false;
    table->max_meals = -1;
    if (!is_atoi_positive_and_int(av[1], &table->num_seats) \
    || !is_atoi_positive_and_int(av[2], &table->to_die) \
    || !is_atoi_positive_and_int(av[3], &table->to_eat) \
    || !is_atoi_positive_and_int(av[4], &table->to_sleep) \
    || ((ac == 6) \
    && !is_atoi_positive_and_int(av[5], &table->max_meals)))
        return (0);
    table->forks = NULL;
    table->check_death = NULL;
    table->start_execution = NULL;
    table->exit_signal = NULL;
    table->all_meals = NULL;
    table->philo_pids = NULL;
    
    table->semafork_name = ft_strdup("semaforks");
    table->semadeath_name = ft_strdup("semadeath");
    table->semaexec_name = ft_strdup("semaexec");
    table->semaexit_name = ft_strdup("semaexit");
    table->semafull_name = ft_strdup("semaout");
    
    if (!table->semafork_name || !table->semadeath_name || !table->semaexec_name || !table->semaexit_name || !table->semafull_name)
        return (0);
    sem_unlink(table->semafork_name);
    sem_unlink(table->semadeath_name);
    sem_unlink(table->semaexec_name);
    sem_unlink(table->semaexit_name);
    sem_unlink(table->semafull_name); 
    
    table->forks = sem_open(table->semafork_name, O_CREAT, 0644, table->num_seats);
    table->check_death = sem_open(table->semadeath_name, O_CREAT, 0644, 1);
    table->start_execution = sem_open(table->semaexec_name, O_CREAT, 0644, 1);
    table->exit_signal = sem_open(table->semaexit_name, O_CREAT, 0644, 0);
    table->all_meals = sem_open(table->semafull_name, O_CREAT, 0644, 1);
    
    if (table->forks == SEM_FAILED \
    || table->check_death == SEM_FAILED \
    || table->start_execution == SEM_FAILED \
    || table->exit_signal == SEM_FAILED
    || table->all_meals == SEM_FAILED)
    {
        perror("sem_open");
        
        return (0);
    }
        
    table->philo_pids = malloc(sizeof(*table->philo_pids) * (table->num_seats));
    if (!table->philo_pids)
        return (0);
        
    memset(table->philo_pids, '\0', sizeof(*table->philo_pids) * table->num_seats);
    
    table->last_good_philo = table->num_seats;

    table->philo.self_monitor = 0;
    table->philo.died = 0;
    table->philo.last_meal_start = 0;
    table->philo.meals_i_had = 0;
    table->philo.must_exit = 0;

    return (1);
}

int clean_table(t_table *table, bool wait, int exit_status)
{
    int i;
    int j;
    int status;
    int sent;
    
    //printf("%-10ld  philo %d starting cleanup \n", milisec_epoch() - table->open_time, table->philo.my_id);
    if (table->philo_pids)
    {
        if (wait)
        {
            i = 0;
            sent = 0;
            //printf("cleaning of philo %d posting exit signals\n", table->philo.my_id);
            while (i < table->last_good_philo)
            {
                
                waitpid(table->philo_pids[i++], &status, 0);
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0 && !sent)
                {
                    sent = 1;
                    j = 0;
                    while (j++ < table->last_good_philo)
                        sem_post(table->exit_signal);
                }
                if (!sent)
                {
                    j = 0;
                    while (j++ < table->last_good_philo)
                        sem_post(table->exit_signal);                    
                }
                //sem_post(table->exit_signal);
                //printf("%-10ld son %d arrived\n", milisec_epoch() - table->open_time, i);
            }
                
            //printf("everybody home\n");
            //printf("all children arrived\n");
        }
        free(table->philo_pids);
        
    }

    sem_close(table->forks);
    sem_close(table->check_death);
    sem_close(table->start_execution);
    sem_close(table->exit_signal);
    sem_close(table->all_meals);
    sem_unlink(table->semafork_name);
    sem_unlink(table->semadeath_name);
    sem_unlink(table->semaexec_name);
    sem_unlink(table->semaexit_name);
    sem_unlink(table->semafull_name);

    if (table->semafork_name)
        ft_free_set_null(&table->semafork_name);
    if (table->semadeath_name)
        ft_free_set_null(&table->semadeath_name);
    if (table->semaexec_name)
        ft_free_set_null(&table->semaexec_name);
    if (table->semaexit_name)
        ft_free_set_null(&table->semaexit_name);
    if (table->semafull_name)
        ft_free_set_null(&table->semafull_name);
    exit(exit_status);
    return (1);
}
