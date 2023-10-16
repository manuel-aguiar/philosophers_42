/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_death.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:18:47 by codespace         #+#    #+#             */
/*   Updated: 2023/10/16 22:47:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*the_end_of_life(t_table *table, time_t time)
{
	time += milisec_epoch() * (time == 0);

	printf("%-10ld %-5d %s\n", time - table->open_time, 
		table->philo.my_id, PRINT_DEATH);
	sem_post(table->someone_died);
	return (NULL);
}

static int	i_am_dead(t_table *table, t_philo *philo)
{
	if (philo->last_meal_start != 0)
	{
		philo->cur_time = milisec_epoch();
		if (philo->cur_time - philo->last_meal_start >= table->to_die)
		{
			sem_wait(table->check_death);
			philo->died = 1;
			return (1);
		}
	}
	return (0);
}

void	*monitor_my_own_death(void *mytable)
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)mytable;
	philo = &table->philo;
	if (table->to_die == 0)
	{
		philo->died = 1;
		return (the_end_of_life(table, 0));
	}
	while (1)
	{
		sem_wait(philo->my_meal);
		if (i_am_dead(table, philo))
			return (the_end_of_life(table, philo->cur_time));
		sem_post(philo->my_meal);
		usleep(TACTICAL_WAIT);
	}
	return (NULL);
}
