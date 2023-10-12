/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_the_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:33:22 by codespace         #+#    #+#             */
/*   Updated: 2023/10/12 10:45:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	prepare_forks_and_ids(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_seats)
	{
		table->philo.my_id = i + 1;
		table->philo_pids[i] = fork();
		if (table->philo_pids[i] == -1)
			return (0);
		if (!table->philo_pids[i])
			the_life_of_a_philosopher(table, &table->philo);
		i++;
	}
	table->philo.my_id = 0;
	return (1);
}

int	prepare_table(t_table *table, int ac, char **av)
{
	table->max_meals = -1;
	if (!is_atoi_positive_and_int(av[1], &table->num_seats)
		|| !is_atoi_positive_and_int(av[2], &table->to_die)
		|| !is_atoi_positive_and_int(av[3], &table->to_eat)
		|| !is_atoi_positive_and_int(av[4], &table->to_sleep) || ((ac == 6)
			&& !is_atoi_positive_and_int(av[5], &table->max_meals)))
		return (0);
	table->philo_pids = NULL;
	sem_unlink(SEMAFORK);
	sem_unlink(SEMADEATH);
	sem_unlink(SEMAEXEC);
	table->forks = sem_open(SEMAFORK, O_CREAT, 0644,
			table->num_seats);
	table->check_death = sem_open(SEMADEATH, O_CREAT, 0644, 1);
	table->start_execution = sem_open(SEMAEXEC, O_CREAT, 0644, 1);
	if (table->forks == SEM_FAILED || table->check_death == SEM_FAILED
		|| table->start_execution == SEM_FAILED)
		return (0);
	table->philo_pids = malloc(sizeof(*table->philo_pids) * (table->num_seats));
	if (!table->philo_pids)
		return (0);
	memset(table->philo_pids, '\0', sizeof(*table->philo_pids)
		* table->num_seats);
	table->philo.self_monitor = 0;
	table->philo.died = 0;
	table->philo.last_meal_start = 0;
	table->philo.meals_i_had = 0;
	return (1);
}

int	clean_table(t_table *table, bool wait, int exit_status)
{
	int	i;

	if (table->philo_pids)
	{
		if (wait)
		{
			i = 0;
			while (i < table->num_seats)
				waitpid(table->philo_pids[i++], NULL, 0);
		}
		free(table->philo_pids);
	}
	sem_close(table->forks);
	sem_close(table->check_death);
	sem_close(table->start_execution);
	sem_unlink(SEMAFORK);
	sem_unlink(SEMADEATH);
	sem_unlink(SEMAEXEC);
	exit(exit_status);
	return (1);
}
