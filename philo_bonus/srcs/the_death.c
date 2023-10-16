/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_death.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:18:47 by codespace         #+#    #+#             */
/*   Updated: 2023/10/16 23:33:51 by mmaria-d         ###   ########.fr       */
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

int	i_am_dead(t_table *table, t_philo *philo)
{
	if (philo->last_meal_start != 0)
	{
		philo->cur_time = milisec_epoch();
		//printf("%-10ld philo %d checking death\n", philo->cur_time - philo->last_meal_start, philo->my_id);
		if (philo->cur_time - philo->last_meal_start >= table->to_die)
		{
			sem_wait(table->check_death);
			//printf("philo %d died and locked death\n", philo->my_id);
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
		//sem_wait(table->check_death);
		
		if (i_am_dead(table, philo))
			return (the_end_of_life(table, 0));
		
		//sem_post(table->check_death);
		sem_post(philo->my_meal);
		usleep(TACTICAL_WAIT);
	}
	return (NULL);
}
