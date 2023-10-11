/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:36 by codespace         #+#    #+#             */
/*   Updated: 2023/10/11 16:27:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_sleep(t_table *table, t_philo *philo, time_t end_sleep)
{
	while (1)
	{
		sem_wait(table->check_death);
		if (philo->died)
		{
			sem_post(table->check_death);
			return (0);
		}
		else
			sem_post(table->check_death);
		if (milisec_epoch() >= end_sleep)
			break ;
		usleep(TACTICAL_WAIT);
	}
	return (1);
}

int	time_to_eat(t_table *table, t_philo *philo)
{
	int	ret;

	sem_wait(table->check_death);
	if (!philo->died)
	{
		broadcast_life_state(table, PRINT_EATING, 0);
		sem_post(table->check_death);
		if (!philo_sleep(table, philo, milisec_epoch() + table->to_eat))
		{
			sem_post(table->forks);
			sem_post(table->forks);
			return (0);
		}
	}
	else
		sem_post(table->check_death);
	sem_wait(table->check_death);
	philo->meals_i_had += 1 * (!philo->died);
	ret = (philo->meals_i_had == table->max_meals) + philo->died;
	sem_post(table->forks);
	sem_post(table->forks);
	sem_post(table->check_death);
	return (!ret);
}

int	time_to_sleep(t_table *table, t_philo *philo)
{
	sem_wait(table->check_death);
	if (!philo->died)
	{
		broadcast_life_state(table, PRINT_SLEEPING, 0);
		sem_post(table->check_death);
		if (!philo_sleep(table, philo, milisec_epoch() + table->to_sleep))
			return (0);
		return (1);
	}
	else
	{
		sem_post(table->check_death);
		return (0);
	}
	return (1);
}

int	time_to_think(t_table *table, t_philo *philo)
{
	sem_wait(table->check_death);
	if (!philo->died)
	{
		broadcast_life_state(table, PRINT_THINKING, 0);
		sem_post(table->check_death);
		return (1);
	}
	else
	{
		sem_post(table->check_death);
		return (0);
	}
	return (1);
}
