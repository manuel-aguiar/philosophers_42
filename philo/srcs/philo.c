/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:24:04 by codespace         #+#    #+#             */
/*   Updated: 2023/10/11 16:26:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	a_philosopher_has_died(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->check_death);
	table->cur_time = milisec_epoch();
	i = 0;
	while (i < table->num_seats)
	{
		if (!(table->philos[i].last_meal_start == 0
				|| table->philos[i].meals_i_had == table->max_meals))
		{
			if (table->cur_time
				- table->philos[i].last_meal_start >= table->to_die)
			{
				table->exit_table = 1;
				broadcast_life_state(table, &table->philos[i], PRINT_DEATH,
					table->cur_time - table->open_time);
				pthread_mutex_unlock(&table->check_death);
				return (1);
			}
		}
		i++;
	}
	pthread_mutex_unlock(&table->check_death);
	return (0);
}

int	all_philos_are_full(t_table *table)
{
	int	res;

	res = 0;
	if (table->max_meals == -1)
		return (res);
	pthread_mutex_lock(&table->check_death);
	if (table->finished_eating == table->num_seats)
	{
		res = 1;
		table->exit_table = 1;
	}
	pthread_mutex_unlock(&table->check_death);
	return (res);
}

int	open_hell_s_kitchen(t_table *table)
{
	if (table->max_meals == 0)
		return (1);
	if (table->to_die == 0)
		table->exit_table = 1;
	pthread_mutex_lock(&table->start_execution);
	if (!prepare_threads_and_ids(table))
	{
		table->init_failed = 1;
		printf("philo: pthread: last thread was %d\n", table->last_good_thread);
		pthread_mutex_unlock(&table->start_execution);
		return (0);
	}
	table->open_time = milisec_epoch();
	pthread_mutex_unlock(&table->start_execution);
	while (1)
	{
		if (a_philosopher_has_died(table) || all_philos_are_full(table))
			break ;
		usleep(TACTICAL_WAIT);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac < 5 || ac > 6)
	{
		printf("philo: invalid arguments\n");
		return (0);
	}
	if (!prepare_table(&table, ac, av))
	{
		printf("philo: syscall failed\n");
		return (0);
	}
	open_hell_s_kitchen(&table);
	clean_table(&table);
	return (0);
}
