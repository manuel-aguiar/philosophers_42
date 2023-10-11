/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_life.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:31:13 by codespace         #+#    #+#             */
/*   Updated: 2023/10/11 16:31:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	the_beginning_of_life(t_table *table, t_philo *philo);

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
			table->cur_time = milisec_epoch();
			if (table->cur_time - philo->last_meal_start >= table->to_die)
			{
				philo->died = 1;
				broadcast_life_state(table, PRINT_DEATH, table->cur_time
					- table->open_time);
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
	sem_wait(table->start_execution);
	sem_post(table->start_execution);
	if (pthread_create(&philo->self_monitor, NULL, monitor_death_or_full,
			table))
		clean_table(table, false, EXIT_FAILURE);
	table->death_monitor = 0;
	table->full_monitor = 0;
	sem_wait(table->check_death);
	philo->last_meal_start = milisec_epoch();
	sem_post(table->check_death);
	return (1);
}
