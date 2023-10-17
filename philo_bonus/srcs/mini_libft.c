/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_libft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:30:50 by codespace         #+#    #+#             */
/*   Updated: 2023/10/17 15:02:35 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_free_set_null(void *ptr)
{
	
	unsigned char	**to_free;

	to_free = (unsigned char **)ptr;
	if (*to_free)
		free(*to_free);
	*to_free = NULL;
	return (NULL);
}

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_isspace(int c)
{
	return ((c >= '\t' && c <= '\r') || c == ' ');
}

static int	atoi_overf(int res, char next, int *sign)
{
	int	max;

	max = INT_MAX / 10;
	if (res < max)
		return (1);
	if (res > max)
	{
		*sign = 0;
		return (0);
	}
	else if (res == max)
	{
		if (*sign == 1 && next > INT_MAX % 10 + '0')
		{
			*sign = 0;
			return (0);
		}
		if (*sign == -1 && next > -(INT_MIN % 10) + '0')
		{
			*sign = 0;
			return (0);
		}
	}
	return (1);
}

int	is_atoi_positive_and_int(char *str, int *place_res)
{
	int	res;
	int	sign;

	if (place_res)
		*place_res = 0;
	while (*str && ft_isspace(*str))
		str++;
	sign = 1;
	if (*str && *str == '-')
		sign = -1;
	if (*str && (*str == '-' || *str == '+'))
		str++;
	if (!ft_isdigit(*str))
		return (0);
	res = 0;
	while (*str && ft_isdigit(*str) && atoi_overf(res, *str, &sign))
		res = res * 10 - '0' + *str++;
	if (!sign || (*str && !ft_isdigit(*str)))
		return (0);
	if (place_res)
		*place_res = res;
	return (sign == 1);
}
