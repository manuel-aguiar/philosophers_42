/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:26:53 by codespace         #+#    #+#             */
/*   Updated: 2023/10/12 11:42:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

time_t	milisec_epoch(void)
{
	struct timeval	cur;

	if (gettimeofday(&cur, NULL) == -1)
		return (0);
	return (cur.tv_sec * 1000 + cur.tv_usec / 1000);
}

int	broadcast_life_state(t_table *table, t_philo *philo, char *state,
		time_t time_elapsed)
{
	if (!time_elapsed)
	{
		philo->cur_time = milisec_epoch();
		time_elapsed = philo->cur_time - table->open_time;
	}
	printf("%-10ld %-5d %s\n", time_elapsed, philo->my_id, state);
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
