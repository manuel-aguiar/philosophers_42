/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:36 by codespace         #+#    #+#             */
/*   Updated: 2023/10/16 15:31:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_sleep(time_t end_sleep)
{
	while (1)
	{
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
		philo->last_meal_start = philo->cur_time;
		sem_post(table->check_death);
		philo_sleep(milisec_epoch() + table->to_eat);
	}
	else
		sem_post(table->check_death);
	ret = !philo->died;
	if ((++philo->meals_i_had * ret == table->max_meals))
		sem_post(table->check_full);
	sem_post(table->forks);
	sem_post(table->forks);
	sem_post(table->check_death);
	return (ret);
}

int	time_to_sleep(t_table *table, t_philo *philo)
{
	sem_wait(table->check_death);
	if (!philo->died)
	{
		broadcast_life_state(table, PRINT_SLEEPING, 0);
		sem_post(table->check_death);
		philo_sleep(milisec_epoch() + table->to_sleep);
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
