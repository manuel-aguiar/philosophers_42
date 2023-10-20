/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:39 by codespace         #+#    #+#             */
/*   Updated: 2023/10/20 10:14:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_first_fork(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
		broadcast_life_state(table, philo, PRINT_FORK, 0);
	else
	{
		pthread_mutex_unlock(philo->first_fork);
		pthread_mutex_unlock(&table->check_death);
		return (0);
	}
	pthread_mutex_unlock(&table->check_death);
	return (1);
}

int	take_second_fork(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(philo->second_fork);
	pthread_mutex_lock(&table->check_death);
	if (!table->exit_table)
		broadcast_life_state(table, philo, PRINT_FORK, 0);
	else
	{
		pthread_mutex_unlock(philo->second_fork);
		pthread_mutex_unlock(philo->first_fork);
		pthread_mutex_unlock(&table->check_death);
		return (0);
	}
	pthread_mutex_unlock(&table->check_death);
	return (1);
}
