/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_death.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:18:47 by codespace         #+#    #+#             */
/*   Updated: 2023/10/12 12:20:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	goodbye_everybody(void)
{
	sem_unlink(SEMAFORK);
	sem_unlink(SEMADEATH);
	sem_unlink(SEMAEXEC);
	kill(0, SIGKILL);
}

static void	the_end_of_life(t_table *table, time_t time)
{
	time += milisec_epoch() * (time == 0);
	broadcast_life_state(table, PRINT_DEATH, time
		- table->open_time);
	goodbye_everybody();
}

static int	i_am_dead(t_table *table, t_philo *philo)
{
	if (!(philo->last_meal_start == 0
			|| philo->meals_i_had == table->max_meals))
	{
		philo->cur_time = milisec_epoch();
		if (philo->cur_time - philo->last_meal_start >= table->to_die)
			return (1);
	}
	return (0);
}

void	*monitor_death_or_full(void *mytable)
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)mytable;
	philo = &table->philo;
	if (table->to_die == 0)
	{
		sem_wait(table->check_death);
		the_end_of_life(table, 0);
	}
	while (1)
	{
		sem_wait(table->check_death);
		if (i_am_dead(table, philo))
			the_end_of_life(table, philo->cur_time);
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
