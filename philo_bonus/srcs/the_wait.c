/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:36 by codespace         #+#    #+#             */
/*   Updated: 2023/10/12 12:21:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	time_to_eat(t_table *table, t_philo *philo)
{
	int	ret;

	sem_wait(table->check_death);
	if (!philo->died)
	{
		broadcast_life_state(table, PRINT_EATING, 0);
		philo->last_meal_start = philo->cur_time;
		sem_post(table->check_death);
		usleep(table->to_eat * 1000);
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
		usleep(table->to_sleep * 1000);
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
