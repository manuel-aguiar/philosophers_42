/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:39 by codespace         #+#    #+#             */
/*   Updated: 2023/10/24 11:40:27 by codespace        ###   ########.fr       */
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

/*

Tests to show fork stealing:

take_first_fork
before lock:
	printf("%-10ld %-5d wanting to grab fork %d\n", milisec_epoch() 
	- table->open_time, philo->my_id, philo->my_id * (philo->my_id % 2 == 1) 
	+ ((philo->my_id + 1) * (philo->my_id != table->num_seats) + 1 * 
	(philo->my_id == table->num_seats))  * (philo->my_id % 2 == 0)) ;

after lock:
	printf("%-10ld %-5d managed to get fork %d\n", milisec_epoch() 
	- table->open_time, philo->my_id, philo->my_id * (philo->my_id % 2 == 1) 
	+ ((philo->my_id + 1) * (philo->my_id != table->num_seats) + 1 * 
	(philo->my_id == table->num_seats))  * (philo->my_id % 2 == 0)) ;


take_second_fork
before lock:
	printf("%-10ld %-5d wanting to grab fork %d\n", milisec_epoch() 
	- table->open_time, philo->my_id, philo->my_id * (philo->my_id % 2 == 0) 
	+ ((philo->my_id + 1) * (philo->my_id != table->num_seats) + 1 * 
	(philo->my_id == table->num_seats))  * (philo->my_id % 2 == 1)) ;


after lock:
	printf("%-10ld %-5d managed to get fork %d\n", milisec_epoch() 
	- table->open_time, philo->my_id, philo->my_id * (philo->my_id % 2 == 0) 
	+ ((philo->my_id + 1) * (philo->my_id != table->num_seats) + 1 * 
	(philo->my_id == table->num_seats))  * (philo->my_id % 2 == 1)) ;

*/
