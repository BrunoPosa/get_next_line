/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:29:02 by bposa             #+#    #+#             */
/*   Updated: 2024/03/14 16:37:00 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	my_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	s = NULL;
	return (i);
}

void	*my_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	i = 0;
	if (src == 0 && dst == 0)
		return (NULL);
	while (i +1 < n)
	{
		d[i] = s[i];
		i++;
	}
	if (n != 0)
		d[i] = '\0';
	return (d);
}

void	my_bzero(char *s)
{
	ssize_t	i;

	i = 0;
	while (s[i] != 0)
	{
		s[i] = 0;
		i++;
	}
}
