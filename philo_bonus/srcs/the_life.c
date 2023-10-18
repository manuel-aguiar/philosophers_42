/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_life.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:31:13 by codespace         #+#    #+#             */
/*   Updated: 2023/10/18 15:31:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	the_beginning_of_life(t_table *table, t_philo *philo);
static void	the_life_of_a_lonely_philo(t_table *table, t_philo *philo);

void	the_life_of_a_philosopher(t_table *table, t_philo *philo)
{
	int	to_think;

	if (!the_beginning_of_life(table, philo))
		return ;
	if (table->num_seats == 1)
		return (the_life_of_a_lonely_philo(table, philo));
	to_think = (table->to_die - table->to_eat - table->to_sleep) / 5;
	if (philo->my_id % 2 == 0)
		philo_sleep(milisec_epoch() + to_think + (to_think <= 0));
	while (1)
	{
		if (!take_first_fork(table, philo) || !take_second_fork(table, philo)
			|| !time_to_eat(table, philo) || !time_to_sleep(table, philo)
			|| !time_to_think(table, philo))
			break ;
	}
	pthread_join(table->philo.self_monitor, NULL);
	clean_table(table, false, EXIT_SUCCESS);
}

static void	the_life_of_a_lonely_philo(t_table *table, t_philo *philo)
{
	sem_wait(table->forks);
	sem_wait(philo->my_meal);
	if (!philo->died)
	{
		broadcast_life_state(table, PRINT_FORK, 0);
		sem_post(philo->my_meal);
		usleep(table->to_die * 1000 * 2 + 1000000);
	}
	else
		sem_post(philo->my_meal);
	sem_post(table->forks);
}

static int	philo_set_semaphores(t_table *table, t_philo *philo)
{
	philo->my_death_check = sem_open(SEMADEATH, O_CREAT, 0644, 1);
	ft_strcpy(philo->meal_name, SEMAMEAL);
	positive_int_to_str(&philo->meal_name[6], philo->my_id);
	sem_unlink(philo->meal_name);
	philo->my_meal = sem_open(philo->meal_name, O_CREAT, 0644, 1);
	if (philo->my_meal == SEM_FAILED \
	|| philo->my_death_check == SEM_FAILED)
	{
		write_stderr("philo_bonus: sem_open: failed\n");
		sem_post(table->someone_died);
		return (0);
	}
	return (1);
}

static int	the_beginning_of_life(t_table *table, t_philo *philo)
{
	sem_wait(table->check_full);
	sem_wait(table->someone_died);
	if (!philo_set_semaphores(table, philo))
		return (0);
	sem_wait(philo->my_meal);
	sem_wait(philo->my_death_check);
	philo_sleep(table->open_time);
	if (pthread_create(&philo->self_monitor, NULL, monitor_my_own_death,
			table))
	{
		write_stderr("philo_bonus: pthread: failed\n");
		sem_post(table->someone_died);
		return (0);
	}
	philo->last_meal_start = milisec_epoch();
	sem_post(philo->my_death_check);
	sem_post(philo->my_meal);
	return (1);
}

// to die > to_eat + to_sleep + "FRICCAO"

// to die > to_eat * (2 + numero de filosofos % 2) + "FRICCAO"