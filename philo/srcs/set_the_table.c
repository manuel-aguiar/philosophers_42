/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_the_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:22 by codespace         #+#    #+#             */
/*   Updated: 2023/10/11 16:26:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	prepare_threads_and_ids(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_seats)
	{
		table->philos[i].table = table;
		table->philos[i].my_id = i + 1;
		table->philos[i].first_fork = &table->forks[i];
		table->philos[i].second_fork = &table->forks[i - 1 + table->num_seats
			* (i == 0)];
		if (i % 2)
			fork_swap(&table->philos[i].first_fork,
				&table->philos[i].second_fork);
		if (pthread_create(&table->threads[i], NULL, the_life_of_a_philosopher,
				&table->philos[i]))
		{
			table->last_good_thread = i;
			table->init_failed = true;
			return (0);
		}
		i++;
	}
	return (1);
}

void	fork_swap(pthread_mutex_t **left, pthread_mutex_t **right)
{
	pthread_mutex_t	*temp;

	temp = *left;
	*left = *right;
	*right = temp;
}

int	table_place_forks_and_detectors(t_table *table)
{
	int	i;

	memset(table->philos, '\0', sizeof(*table->philos) * table->num_seats);
	memset(table->forks, '\0', sizeof(*table->forks) * table->num_seats);
	memset(&table->check_death, '\0', sizeof(table->check_death));
	memset(&table->start_execution, '\0', sizeof(table->start_execution));
	if (pthread_mutex_init(&table->start_execution, NULL)
		|| pthread_mutex_init(&table->check_death, NULL))
	{
		printf("mutex failed\n");
		return (clean_table(table));
	}
	i = 0;
	while (i < table->num_seats)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
			return (clean_table(table));
		i++;
	}
	return (1);
}

int	prepare_table(t_table *table, int ac, char **av)
{
	table->init_failed = false;
	table->max_meals = -1;
	if (!is_atoi_positive_and_int(av[1], &table->num_seats)
		|| !is_atoi_positive_and_int(av[2], &table->to_die)
		|| !is_atoi_positive_and_int(av[3], &table->to_eat)
		|| !is_atoi_positive_and_int(av[4], &table->to_sleep) || ((ac == 6)
			&& !is_atoi_positive_and_int(av[5], &table->max_meals)))
		return (0);
	table->threads = malloc(sizeof(*table->threads) * (table->num_seats));
	if (table->threads)
		memset(table->threads, '\0', sizeof(*table->threads)
			* table->num_seats);
	table->forks = malloc(sizeof(*table->forks) * (table->num_seats));
	table->philos = malloc(sizeof(*table->philos) * (table->num_seats));
	if (!table->threads || !table->forks || !table->philos)
		return (clean_table(table));
	table->finished_eating = 0;
	table->exit_table = 0;
	table->last_good_thread = table->num_seats;
	return (table_place_forks_and_detectors(table));
}

int	clean_table(t_table *table)
{
	int	i;

	if (table->threads)
	{
		i = 0;
		while (i < table->last_good_thread)
		{
			if (table->threads[i])
				pthread_join(table->threads[i], NULL);
			i++;
		}
		ft_free_set_null(&table->threads);
	}
	if (table->philos)
		ft_free_set_null(&table->philos);
	if (table->forks)
	{
		i = 0;
		while (i < table->num_seats)
			pthread_mutex_destroy(&table->forks[i++]);
		ft_free_set_null(&table->forks);
	}
	pthread_mutex_destroy(&table->check_death);
	pthread_mutex_destroy(&table->start_execution);
	return (0);
}
