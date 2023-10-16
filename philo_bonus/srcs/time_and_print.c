/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:27 by codespace         #+#    #+#             */
/*   Updated: 2023/10/16 22:02:09 by mmaria-d         ###   ########.fr       */
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
	if (!time_elapsed)
	{
		table->philo.cur_time = milisec_epoch();
		time_elapsed = table->philo.cur_time - table->open_time;
	}
	sem_wait(table->check_death);
	printf("%-10ld %-5d %s\n", time_elapsed, table->philo.my_id, state);
	sem_post(table->check_death);
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
