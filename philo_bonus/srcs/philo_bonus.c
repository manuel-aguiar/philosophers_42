/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:32:06 by codespace         #+#    #+#             */
/*   Updated: 2023/10/18 11:40:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philocide(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_seats)
	{
		if (table->philo_pids[i])
			kill(table->philo_pids[i], SIGKILL);
		i++;
	}
}

void	*full_monitor(void *mytable)
{
	t_table	*table;
	int		i;

	table = (t_table *)mytable;
	i = -1;
	while (++i < table->num_seats)
		sem_wait(table->check_full);
	sem_post(table->someone_died);
	return (NULL);
}

void	*death_monitor(void *mytable)
{
	t_table	*table;
	int		i;

	table = (t_table *)mytable;
	sem_wait(table->someone_died);
	philocide(table);
	i = -1;
	while (++i < table->num_seats)
		sem_post(table->check_full);
	return (NULL);
}

int	open_hell_s_kitchen(t_table *table)
{
	int	i;

	if (table->max_meals == 0)
		return (1);
	table->open_time = milisec_epoch() + (table->num_seats \
	* TACTICAL_WAIT * START_NUM / START_DIV) / 1000;
	sem_wait(table->check_death);
	if (!prepare_forks_and_ids(table))
	{
		philocide(table);
		return (0);
	}
	i = -1;
	while (++i < table->num_seats)
		usleep(TACTICAL_WAIT);
	if (pthread_create(&table->full_monitor, NULL, full_monitor, table) \
	|| pthread_create(&table->death_monitor, NULL, death_monitor, table))
	{
		philocide(table);
		return (0);
	}
	sem_post(table->check_death);
	return (1);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac < 5 || ac > 6)
	{
		write_stderr("philo_bonus: invalid number of arguments\n");
		return (0);
	}
	if (prepare_table(&table, ac, av))
		open_hell_s_kitchen(&table);
	clean_table(&table, true, EXIT_SUCCESS);
	return (0);
}

/*

valgrind --track-fds=yes --trace-children=yes 
--leak-check=full ./philo_bonus 4 310 200 100

stdbuf -o0 ./philo_bonus 4 400 200 200 10 > test1.txt


*/