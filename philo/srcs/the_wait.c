/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:46 by codespace         #+#    #+#             */
/*   Updated: 2023/10/18 14:10:41 by codespace        ###   ########.fr       */
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
	int	ret;

	pthread_mutex_lock(&table->check_death);
	ret = !table->exit_table;
	table->finished_eating += (++philo->meals_i_had == table->max_meals) \
		* ret;
	pthread_mutex_unlock(philo->second_fork);
	pthread_mutex_unlock(philo->first_fork);
	pthread_mutex_unlock(&table->check_death);
	return (ret);
}

int	time_to_eat(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
	{
		broadcast_life_state(table, philo, PRINT_EATING, 0);
		philo->last_meal_start = philo->cur_time;
		pthread_mutex_unlock(&table->check_death);
		if (!philo_sleep(table, milisec_epoch() + table->to_eat))
		{
			pthread_mutex_unlock(philo->second_fork);
			pthread_mutex_unlock(philo->first_fork);
			return (0);
		}
	}
	else
		pthread_mutex_unlock(&table->check_death);
	return (update_meals(table, philo));
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
	int	think;
	
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
	{
		broadcast_life_state(table, philo, PRINT_THINKING, 0);
		think = (table->to_die + philo->last_meal_start - philo->cur_time) / 5;
		pthread_mutex_unlock(&table->check_death);
		if (!philo_sleep(table, milisec_epoch() + think))
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
