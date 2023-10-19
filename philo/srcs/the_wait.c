/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:46 by codespace         #+#    #+#             */
/*   Updated: 2023/10/19 16:17:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_sleep(t_table *table, time_t end_sleep)
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

int	update_meals(t_table *table, t_philo *philo)
{
	table->finished_eating += (++philo->meals_i_had == table->max_meals);
	if (table->finished_eating == table->num_seats)
	{
		table->exit_table = 1;
		pthread_mutex_unlock(philo->second_fork);
		pthread_mutex_unlock(philo->first_fork);
		pthread_mutex_unlock(&table->check_death);
		return (0);
	}
	return (1);
}

int	time_to_eat(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
	{
		broadcast_life_state(table, philo, PRINT_EATING, 0);
		philo->last_meal_start = philo->cur_time;
		if (!update_meals(table, philo))
			return (0);
		pthread_mutex_unlock(&table->check_death);
		if (!philo_sleep(table, milisec_epoch() + table->to_eat))
		{
			pthread_mutex_unlock(philo->second_fork);
			pthread_mutex_unlock(philo->first_fork);
			return (0);
		}
		pthread_mutex_unlock(philo->second_fork);
		pthread_mutex_unlock(philo->first_fork);
	}
	else
		pthread_mutex_unlock(&table->check_death);
	return (1);
}

int	time_to_sleep(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
		broadcast_life_state(table, philo, PRINT_SLEEPING, 0);
	else
	{
		pthread_mutex_unlock(&table->check_death);
		return (0);
	}
	pthread_mutex_unlock(&table->check_death);
	if (!philo_sleep(table, milisec_epoch() + table->to_sleep))
		return (0);
	return (1);
}

int	time_to_think(t_table *table, t_philo *philo)
{
	int	to_think;

	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
	{
		broadcast_life_state(table, philo, PRINT_THINKING, 0);
		to_think = (table->to_die + philo->last_meal_start \
		- philo->cur_time + (table->to_eat - table->to_sleep) \
		* (table->to_eat > table->to_sleep)) / 5;
		pthread_mutex_unlock(&table->check_death);
		if (!philo_sleep(table, milisec_epoch() + to_think + (to_think <= 0)))
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
