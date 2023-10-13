/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:32:06 by codespace         #+#    #+#             */
/*   Updated: 2023/10/13 10:15:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	open_hell_s_kitchen(t_table *table)
{
	if (table->max_meals == 0)
		return (1);
	table->open_time = milisec_epoch();
	if (!prepare_forks_and_ids(table))
	{
		write_stderr("philo: fork: failed\n");
		goodbye_everybody();
		return (0);
	}
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