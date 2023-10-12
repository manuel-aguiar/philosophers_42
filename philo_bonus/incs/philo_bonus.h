/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:25:01 by codespace         #+#    #+#             */
/*   Updated: 2023/10/12 10:16:19 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H

# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define TACTICAL_WAIT 100

# define PRINT_FORK "has taken a fork"
# define PRINT_EATING "is eating"
# define PRINT_SLEEPING "is sleeping"
# define PRINT_THINKING "is thinking"
# define PRINT_DEATH "died"

# define SEMAFORK "semafork"
# define SEMADEATH "semadeath"
# define SEMAEXEC "semaexec"

typedef struct s_philo
{
	pthread_t	self_monitor;
	int			died;
	int			my_id;
	int			meals_i_had;
	int			must_exit;
	time_t		last_meal_start;
	time_t		cur_time;
}				t_philo;

typedef struct s_table
{
	t_philo		philo;
	pid_t		*philo_pids;
	sem_t		*forks;
	sem_t		*check_death;
	sem_t		*start_execution;
	bool		init_failed;
	int			last_good_philo;
	time_t		open_time;
	time_t		cur_time;
	int			to_die;
	int			to_eat;
	int			to_sleep;
	int			max_meals;
	int			num_seats;
}				t_table;

enum			e_state
{
	S_FORK,
	S_EATING,
	S_SLEEPING,
	S_THINKING,
	S_DEAD,
};



/* philosophers.c */
int				a_philosopher_has_died(t_table *table);
int				all_philos_are_full(t_table *table);
int				philocide(t_table *table);
int				open_hell_s_kitchen(t_table *table);

/* set_the_table.c */
int				prepare_table(t_table *table, int ac, char **av);
int				clean_table(t_table *table, bool wait, int exit_status);
int				prepare_forks_and_ids(t_table *table);

/* the_life.c */
void			goodbye_everybody(void);
void			the_life_of_a_philosopher(t_table *table, t_philo *philo);
void			the_life_of_a_lonely_philo(t_table *table, t_philo *philo);

/* the_routine.c */
int				take_first_fork(t_table *table, t_philo *philo);
int				take_second_fork(t_table *table, t_philo *philo);

/* the_wait.c */
int				philo_sleep(t_table *table, t_philo *philo, time_t end_sleep);
int				time_to_eat(t_table *table, t_philo *philo);
int				time_to_sleep(t_table *table, t_philo *philo);
int				time_to_think(t_table *table, t_philo *philo);

/* time_and_print.c */
time_t			milisec_epoch(void);
int				broadcast_life_state(t_table *table, char *state,
					time_t time_elapsed);

/* mini_libft.c */
int				is_atoi_positive_and_int(char *str, int *place_res);
void			*ft_free_set_null(void *ptr);

/* ft_strdup.c */
char			*ft_strdup(char *s);
char			*ft_strjoin(char *s1, char *s2);

#endif