/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:24:04 by codespace         #+#    #+#             */
/*   Updated: 2023/10/26 15:43:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	table_warns_someone_died(t_table *table, int index)
{
	table->exit_table = 1;
	broadcast_life_state(table, &table->philos[index], PRINT_DEATH,
		table->cur_time - table->open_time);
	return (1);
}

int	a_philo_died(t_table *table)
{
	time_t	time_since_death;
	int		philo_index;
	int		i;

	philo_index = -1;
	table->cur_time = milisec_epoch();
	i = 0;
	while (i < table->num_seats)
	{
		if (table->philos[i].last_meal_start != 0 && \
		table->cur_time - table->philos[i].last_meal_start >= table->to_die)
		{
			if (philo_index == -1 || table->cur_time \
			- table->philos[i].last_meal_start < time_since_death)
			{
				philo_index = i;
				time_since_death = table->cur_time \
				- table->philos[i].last_meal_start;
			}
		}
		i++;
	}
	if (philo_index == -1)
		return (0);
	return (table_warns_someone_died(table, philo_index));
}

int	either_a_philospher_died_or_all_are_full(t_table *table)
{
	pthread_mutex_lock(&table->check_death);
	if (table->finished_eating == table->num_seats)
	{
		pthread_mutex_unlock(&table->check_death);
		return (1);
	}
	if (a_philo_died(table))
	{
		pthread_mutex_unlock(&table->check_death);
		return (1);
	}
	pthread_mutex_unlock(&table->check_death);
	return (0);
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
		pthread_mutex_unlock(&table->start_execution);
		return (0);
	}
	table->open_time = milisec_epoch();
	pthread_mutex_unlock(&table->start_execution);
	while (1)
	{
		if (either_a_philospher_died_or_all_are_full(table))
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
		write_stderr("philo: invalid number of arguments\n");
		return (0);
	}
	if (prepare_table(&table, ac, av))
		open_hell_s_kitchen(&table);
	clean_table(&table);
	return (0);
}
