/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_life.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:26 by codespace         #+#    #+#             */
/*   Updated: 2023/10/25 12:30:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	the_beginning_of_life(t_table *table);

void	*the_life_of_a_philosopher(void *my_struct)
{
	t_philo	*philo;
	t_table	*table;
	int		to_wait;

	philo = (t_philo *)my_struct;
	table = philo->table;
	if (!the_beginning_of_life(table))
		return (NULL);
	pthread_mutex_lock(&table->check_death);
	philo->last_meal_start = milisec_epoch();
	to_wait = (table->to_eat / 2 + (table->to_eat <= 1)) \
	* (philo->my_id % 2 != 0);
	pthread_mutex_unlock(&table->check_death);
	if (table->num_seats == 1)
		return (the_life_of_a_lonely_philo(table, philo));
	philo_sleep(table, milisec_epoch() + to_wait);
	while (1)
	{
		if (!take_first_fork(table, philo) || !take_second_fork(table, philo)
			|| !time_to_eat(table, philo) || !time_to_sleep(table, philo)
			|| !time_to_think(table, philo))
			break ;
	}
	return (NULL);
}

void	*the_life_of_a_lonely_philo(t_table *table, t_philo *philo)
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
		return (NULL);
	}
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
	{
		broadcast_life_state(table, philo, PRINT_FORK, 0);
		pthread_mutex_unlock(&table->check_death);
		philo_sleep(table, philo->last_meal_start + table->to_die);
	}
	else
		pthread_mutex_unlock(&table->check_death);
	pthread_mutex_unlock(philo->first_fork);
	return (NULL);
}

static int	the_beginning_of_life(t_table *table)
{
	pthread_mutex_lock(&table->start_execution);
	if (table->init_failed)
	{
		pthread_mutex_unlock(&table->start_execution);
		return (0);
	}
	pthread_mutex_unlock(&table->start_execution);
	return (1);
}
