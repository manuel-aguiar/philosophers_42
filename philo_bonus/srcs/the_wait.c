/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:36 by codespace         #+#    #+#             */
/*   Updated: 2023/10/19 16:19:47 by mmaria-d         ###   ########.fr       */
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
	sem_wait(philo->my_meal);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_EATING, 0);
		philo->last_meal_start = philo->cur_time;
		if ((++philo->meals_i_had == table->max_meals))
			sem_post(table->check_full);
		sem_post(philo->my_meal);
		philo_sleep(milisec_epoch() + table->to_eat);
	}
	else
	{
		sem_post(philo->my_meal);
		the_end_of_life(table);
		return (0);
	}
	sem_post(table->forks);
	sem_post(table->forks);
	return (1);
}

int	time_to_sleep(t_table *table, t_philo *philo)
{
	sem_wait(philo->my_meal);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_SLEEPING, 0);
		sem_post(philo->my_meal);
		philo_sleep(milisec_epoch() + table->to_sleep);
	}
	else
	{
		sem_post(philo->my_meal);
		the_end_of_life(table);
		return (0);
	}
	return (1);
}

int	time_to_think(t_table *table, t_philo *philo)
{
	int	to_think;

	sem_wait(philo->my_meal);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_THINKING, 0);
		to_think = (table->to_die + philo->last_meal_start \
		- philo->cur_time  + (table->to_eat - table->to_sleep) \
		* (table->to_eat > table->to_sleep)) / 5;
		sem_post(philo->my_meal);
		philo_sleep(milisec_epoch() + to_think + (to_think <= 0));
		return (1);
	}
	else
	{
		sem_post(philo->my_meal);
		the_end_of_life(table);
		return (0);
	}
	return (1);
}
