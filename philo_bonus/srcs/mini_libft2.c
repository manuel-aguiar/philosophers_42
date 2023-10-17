/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_libft2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:30:50 by codespace         #+#    #+#             */
/*   Updated: 2023/10/17 14:28:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	count_digits(int nb)
{
	int	count;

	if (nb == 0)
		return (1);
	count = 0;
	while (nb)
	{
		count++;
		nb /= 10;
	}
	return (count);
}

void	positive_int_to_str(char *str, int nb)
{
	int	i;

	i = count_digits(nb);
	str[i--] = '\0';
	if (nb == 0)
	{
		str[i] = '0';
		return ;
	}
	while (nb)
	{
		str[i--] = (nb % 10) + '0';
		nb /= 10;
	}
}

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
