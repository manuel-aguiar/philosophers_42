#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_philo  t_philo;

typedef struct  s_table
{

    t_philo         *philos;
    pthread_t       *threads;
    pthread_mutex_t *forks;
    pthread_mutex_t check_death;
    pthread_mutex_t start_execution;
    bool            init_failed;
    int             last_good_thread;
    time_t          open_time;
    time_t          cur_time;
    int             to_die;
    int             to_eat;
    int             to_sleep;
    int             finished_eating;
    int             max_meals;
    int             num_seats;
    int             exit_table;
}               t_table;

typedef struct  s_philo
{
    t_table         *table;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;
    int             my_id;
    int             meals_i_had;
    time_t          last_meal_start;
    time_t          cur_time;
}               t_philo;

enum e_state
{
    S_FORK,
    S_EATING,
    S_SLEEPING,
    S_THINKING,
    S_DEAD,
};

# define TACTICAL_WAIT 100

# define PRINT_FORK "has taken a fork"
# define PRINT_EATING "is eating"
# define PRINT_SLEEPING "is sleeping"
# define PRINT_THINKING "is thinking"
# define PRINT_DEATH "died"

/* philosophers.c */
int     a_philosopher_has_died(t_table *table);
int     all_philos_are_full(t_table *table);
int     open_hell_s_kitchen(t_table *table);

/* set_the_table.c */
int     prepare_table(t_table *table, int ac, char **av);
int     table_place_forks_set_states(t_table *table);
int     clean_table(t_table *table);
void    fork_swap(pthread_mutex_t **left, pthread_mutex_t **right);
int     prepare_threads_and_ids(t_table *table);

/* the_life.c */
void    *the_life_of_a_philosopher(void *my_struct);
void    *the_life_of_a_lonely_philo(t_table *table, t_philo *philo);

/* the_routine.c */
int     take_first_fork(t_table *table, t_philo *philo);
int     take_second_fork(t_table *table, t_philo *philo);

/* the_wait.c */
int     philo_sleep(t_table *table, time_t end_sleep);
int     time_to_eat(t_table *table, t_philo *philo);
int     time_to_sleep(t_table *table, t_philo *philo);
int     time_to_think(t_table *table, t_philo *philo);

/* time_and_print.c */
time_t  milisec_epoch(void);
int     broadcast_life_state(t_table *table, t_philo *philo, \
        char *state, time_t time_elapsed);

/* mini_libft.c */
int     is_atoi_positive_and_int(char *str, int *place_res);
void	*ft_free_set_null(void *ptr);

#endif