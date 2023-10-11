/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:29:35 by codespace         #+#    #+#             */
/*   Updated: 2023/10/11 16:29:35 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*ptr;
	unsigned char	*ptr2;

	ptr = (unsigned char *)dst;
	ptr2 = (unsigned char *)src;
	while (n-- > 0)
		*(ptr++) = *(ptr2++);
	return (dst);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s)
{
	int		len;
	char	*dest;

	len = ft_strlen(s);
	dest = malloc(sizeof(*dest) * (len + 1));
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s, len);
	dest[len] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*new;
	size_t	len_s1;
	size_t	len_s2;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	new = malloc(sizeof(*new) * (len_s1 + len_s2 + 1));
	if (!new)
		return (NULL);
	ft_memcpy(new, s1, len_s1);
	ft_memcpy(&new[len_s1], s2, len_s2);
	new[len_s1 + len_s2] = '\0';
	return (new);
}
