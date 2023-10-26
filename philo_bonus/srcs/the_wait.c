/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:36 by codespace         #+#    #+#             */
/*   Updated: 2023/10/26 18:57:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_sleep(t_table *table, t_philo *philo, time_t end_sleep, bool in_game)
{
	while (1)
	{
		if (in_game)
		{
			sem_wait(philo->my_death);
			if (i_am_dead(table, philo))
			{
				sem_post(philo->my_death);
				the_end_of_life(table);
				return (0);
			}
			sem_post(philo->my_death);
		}
		if (milisec_epoch() >= end_sleep)
			break ;
		usleep(TACTICAL_WAIT);
	}
	return (1);
}

int	time_to_eat(t_table *table, t_philo *philo)
{
	sem_wait(philo->my_death);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_EATING, 0);
		philo->last_meal_start = philo->cur_time;
		if ((++philo->meals_i_had == table->max_meals))
			sem_post(table->check_full);
		sem_post(philo->my_death);
		if (!philo_sleep(table, philo, milisec_epoch() + table->to_eat, true))
		{
			sem_post(table->forks);
			sem_post(table->forks);
			return (0);
		}
	}
	else
	{
		sem_post(philo->my_death);
		the_end_of_life(table);
		return (0);
	}
	sem_post(table->forks);
	sem_post(table->forks);
	return (1);
}

int	time_to_sleep(t_table *table, t_philo *philo)
{
	sem_wait(philo->my_death);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_SLEEPING, 0);
		sem_post(philo->my_death);
		if (!philo_sleep(table, philo, milisec_epoch() + table->to_sleep, true))
			return (0);
		return (1);
	}
	else
	{
		sem_post(philo->my_death);
		the_end_of_life(table);
		return (0);
	}
	return (1);
}

int	time_to_think(t_table *table, t_philo *philo)
{
	time_t	to_think;
	time_t	save_cur;

	sem_wait(philo->my_death);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_THINKING, 0);
		to_think = (table->to_eat - table->to_sleep) \
		* (table->to_eat > table->to_sleep);
		to_think += (table->to_die + philo->last_meal_start \
		- philo->cur_time - to_think) / 5;
		save_cur = philo->cur_time;
		sem_post(philo->my_death);
		if (!philo_sleep(table, philo, save_cur \
		+ to_think * (to_think >= 0), true))
			return (0);
		return (1);
	}
	else
	{
		sem_post(philo->my_death);
		the_end_of_life(table);
		return (0);
	}
	return (1);
}
