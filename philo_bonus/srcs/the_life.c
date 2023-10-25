/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_life.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:31:13 by codespace         #+#    #+#             */
/*   Updated: 2023/10/25 12:30:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	the_beginning_of_life(t_table *table, t_philo *philo);
static int	the_life_of_a_lonely_philo(t_table *table, t_philo *philo);

void	the_life_of_a_philosopher(t_table *table, t_philo *philo)
{
	int	to_wait;

	if (!the_beginning_of_life(table, philo))
		return ;
	to_wait = (table->to_eat / 2 + (table->to_eat <= 1)) \
	* (philo->my_id % 2 != 0);
	philo_sleep(table, philo, milisec_epoch() + to_wait, true);
	while (1)
	{
		take_first_fork(table, philo);
		take_second_fork(table, philo);
		time_to_eat(table, philo);
		time_to_sleep(table, philo);
		time_to_think(table, philo);
	}
}

static int	the_life_of_a_lonely_philo(t_table *table, t_philo *philo)
{
	sem_wait(philo->my_table_print);
	philo_sleep(table, philo, table->open_time, false);
	philo->last_meal_start = milisec_epoch();
	sem_post(philo->my_table_print);
	if (!i_am_dead(table, philo))
	{
		broadcast_life_state(table, PRINT_FORK, 0);
		philo_sleep(table, philo, milisec_epoch() + table->to_die, true);
		sem_post(philo->my_death);
		the_end_of_life(table);
	}
	else
		the_end_of_life(table);
	return (0);
}

static int	philo_set_semaphores(t_table *table, t_philo *philo)
{
	philo->my_table_print = sem_open(SEMADEATH, O_CREAT, 0644, 1);
	ft_strcpy(philo->meal_name, SEMAMEAL);
	positive_int_to_str(&philo->meal_name[6], philo->my_id);
	sem_unlink(philo->meal_name);
	philo->my_death = sem_open(philo->meal_name, O_CREAT, 0644, 1);
	if (philo->my_death == SEM_FAILED \
	|| philo->my_table_print == SEM_FAILED)
	{
		write_stderr("philo_bonus: sem_open: failed\n");
		sem_post(table->someone_died);
		return (0);
	}
	return (1);
}

static int	the_beginning_of_life(t_table *table, t_philo *philo)
{
	time_t	open;

	sem_wait(table->check_full);
	sem_wait(table->someone_died);
	if (!philo_set_semaphores(table, philo))
		return (0);
	sem_wait(philo->my_death);
	if (table->num_seats == 1)
		return (the_life_of_a_lonely_philo(table, philo));
	sem_wait(philo->my_table_print);
	open = table->open_time;
	sem_post(philo->my_table_print);
	philo_sleep(table, philo, open, false);
	sem_wait(philo->my_table_print);
	if (pthread_create(&philo->self_monitor, NULL, monitor_my_own_death,
			table))
	{
		write_stderr("philo_bonus: pthread: failed\n");
		sem_post(table->someone_died);
		return (0);
	}
	philo->last_meal_start = milisec_epoch();
	sem_post(philo->my_table_print);
	sem_post(philo->my_death);
	return (1);
}

// to die > to_eat + to_sleep + "FRICCAO"

// to die > to_eat * (2 + numero de filosofos % 2) + "FRICCAO"