/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:49 by codespace         #+#    #+#             */
/*   Updated: 2023/10/16 17:39:26 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	take_first_fork(t_table *table, t_philo *philo)
{
	sem_wait(table->forks);
	sem_wait(philo->my_death_check);
	if (!philo->died)
		broadcast_life_state(table, PRINT_FORK, 0);
	else
	{
		sem_post(table->forks);
		sem_post(philo->my_death_check);
		return (0);
	}
	sem_post(philo->my_death_check);
	return (1);
}

int	take_second_fork(t_table *table, t_philo *philo)
{
	sem_wait(table->forks);
	sem_wait(philo->my_death_check);
	if (!philo->died)
		broadcast_life_state(table, PRINT_FORK, 0);
	else
	{
		sem_post(table->forks);
		sem_post(table->forks);
		sem_post(philo->my_death_check);
		return (0);
	}
	sem_post(philo->my_death_check);
	return (1);
}
