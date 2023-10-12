/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_life.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:31:13 by codespace         #+#    #+#             */
/*   Updated: 2023/10/12 11:15:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	the_beginning_of_life(t_table *table, t_philo *philo);

void	goodbye_everybody(void)
{
	sem_unlink(SEMAFORK);
	sem_unlink(SEMADEATH);
	sem_unlink(SEMAEXEC);
	kill(0, SIGKILL);
}

void	*monitor_death_or_full(void *mytable)
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)mytable;
	philo = &table->philo;
	while (1)
	{
		sem_wait(table->check_death);
		if (!(philo->last_meal_start == 0
				|| philo->meals_i_had == table->max_meals))
		{
			philo->cur_time = milisec_epoch();
			if (philo->cur_time - philo->last_meal_start >= table->to_die)
			{
				philo->died = 1;
				broadcast_life_state(table, PRINT_DEATH, philo->cur_time
					- table->open_time);
				goodbye_everybody();
				return (NULL);
			}
		}
		if (philo->meals_i_had == table->max_meals)
		{
			philo->died = 1;
			sem_post(table->check_death);
			return (NULL);
		}
		sem_post(table->check_death);
		usleep(TACTICAL_WAIT);
	}
	return (NULL);
}

void	the_life_of_a_philosopher(t_table *table, t_philo *philo)
{
	if (!the_beginning_of_life(table, philo))
		return ;
	if (table->num_seats == 1)
		return (the_life_of_a_lonely_philo(table, philo));
	while (1)
	{
		if (!take_first_fork(table, philo) || !take_second_fork(table, philo)
			|| !time_to_eat(table, philo) || !time_to_sleep(table, philo)
			|| !time_to_think(table, philo))
			break ;
	}
	pthread_join(table->philo.self_monitor, NULL);
	clean_table(table, false, EXIT_SUCCESS);
}

void	the_life_of_a_lonely_philo(t_table *table, t_philo *philo)
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

static int	the_beginning_of_life(t_table *table, t_philo *philo)
{
	sem_wait(table->check_death);
	if (pthread_create(&philo->self_monitor, NULL, monitor_death_or_full,
			table))
	{
		write_stderr("philo_bonus: pthread: failed\n");
		goodbye_everybody();
	}
	philo->last_meal_start = milisec_epoch();
	sem_post(table->check_death);
	return (1);
}
