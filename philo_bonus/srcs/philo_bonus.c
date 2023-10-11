/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:32:06 by codespace         #+#    #+#             */
/*   Updated: 2023/10/11 16:32:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philocide(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->last_good_philo)
	{
		if (table->philo_pids[i])
		{
			kill(table->philo_pids[i], SIGKILL);
			table->philo_pids[i] = 0;
		}
		i++;
	}
	return (1);
}

int	open_hell_s_kitchen(t_table *table)
{
	if (table->max_meals == 0)
		return (1);
	sem_wait(table->start_execution);
	table->open_time = milisec_epoch();
	if (!prepare_forks_and_ids(table))
	{
		printf("philo: fork: last good philo %d\n", table->last_good_philo);
		philocide(table);
		sem_post(table->start_execution);
		return (0);
	}
	sem_post(table->start_execution);
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
		clean_table(&table, true, EXIT_FAILURE);
		return (0);
	}
	open_hell_s_kitchen(&table);
	clean_table(&table, true, EXIT_SUCCESS);
	return (0);
}

/*

valgrind --track-fds=yes --trace-children=yes --leak-check=full ./philo_bonus 4 310 200 100

*/