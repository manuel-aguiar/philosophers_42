/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_the_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:33:22 by codespace         #+#    #+#             */
/*   Updated: 2023/10/26 14:56:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	prepare_table_semaphores(t_table *table);

int	prepare_forks_and_ids(t_table *table)
{
	int	i;

	table->philo.self_monitor = 0;
	table->philo.last_meal_start = 0;
	table->philo.meals_i_had = 0;
	i = 0;
	while (i < table->num_seats)
	{
		table->philo.my_id = i + 1;
		table->philo_pids[i] = fork();
		if (table->philo_pids[i] == -1)
			return (write_stderr("philo_bonus: fork: failed\n"));
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
	table->philo_pids = NULL;
	table->death_monitor = 0;
	table->full_monitor = 0;
	if (!is_atoi_positive_and_int(av[1], &table->num_seats)
		|| !is_atoi_positive_and_int(av[2], &table->to_die)
		|| !is_atoi_positive_and_int(av[3], &table->to_eat)
		|| !is_atoi_positive_and_int(av[4], &table->to_sleep) || ((ac == 6)
			&& !is_atoi_positive_and_int(av[5], &table->max_meals)))
		return (write_stderr("philo_bonus: argument is not an unsigned int\n"));
	if (!table->num_seats)
		return (0);
	table->philo_pids = malloc(sizeof(*table->philo_pids) * (table->num_seats));
	if (!table->philo_pids)
		return (write_stderr("philo_bonus: malloc: failed\n"));
	memset(table->philo_pids, '\0', sizeof(*table->philo_pids)
		* table->num_seats);
	if (!prepare_table_semaphores(table))
		return (0);
	return (1);
}

int	clean_table(t_table *table, bool init_success, int exit_status)
{
	if (init_success)
	{
		if (table->death_monitor)
			pthread_join(table->death_monitor, NULL);
		if (table->full_monitor)
			pthread_join(table->full_monitor, NULL);
	}
	if (table->philo_pids)
		free(table->philo_pids);
	sem_close(table->forks);
	sem_close(table->main_table_print);
	sem_close(table->check_full);
	sem_close(table->someone_died);
	sem_unlink(SEMAFORK);
	sem_unlink(SEMAPRINT);
	sem_unlink(SEMAFULL);
	sem_unlink(SEMADIED);
	exit(exit_status);
	return (1);
}

static int	prepare_table_semaphores(t_table *table)
{
	sem_unlink(SEMAFORK);
	sem_unlink(SEMAPRINT);
	sem_unlink(SEMAEXEC);
	sem_unlink(SEMAFULL);
	sem_unlink(SEMADIED);
	table->forks = sem_open(SEMAFORK, O_CREAT, 0644,
			table->num_seats);
	table->main_table_print = sem_open(SEMAPRINT, O_CREAT, 0644, 1);
	table->check_full = sem_open(SEMAFULL, O_CREAT, 0644,
			table->num_seats);
	table->someone_died = sem_open(SEMADIED, O_CREAT, 0644,
			table->num_seats);
	table->death_monitor = 0;
	table->full_monitor = 0;
	if (table->forks == SEM_FAILED
		|| table->main_table_print == SEM_FAILED
		|| table->check_full == SEM_FAILED
		|| table->someone_died == SEM_FAILED)
		return (write_stderr("philo_bonus: sem_open: failed\n"));
	return (1);
}
