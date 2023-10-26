/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:27 by codespace         #+#    #+#             */
/*   Updated: 2023/10/26 14:57:34 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

time_t	milisec_epoch(void)
{
	struct timeval	cur;

	if (gettimeofday(&cur, NULL) == -1)
		return (0);
	return (cur.tv_sec * 1000 + cur.tv_usec / 1000);
}

int	broadcast_life_state(t_table *table, char *state, time_t time_elapsed)
{
	sem_wait(table->philo.my_table_print);
	if (!time_elapsed)
	{
		table->philo.cur_time = milisec_epoch();
		time_elapsed = table->philo.cur_time - table->open_time;
	}
	printf("%-10ld %-5d %s\n", time_elapsed, table->philo.my_id, state);
	sem_post(table->philo.my_table_print);
	return (1);
}

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	write_stderr(char *text)
{
	write(STDERR_FILENO, text, ft_strlen(text));
	return (0);
}
