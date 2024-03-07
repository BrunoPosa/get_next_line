/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:49:44 by bposa             #+#    #+#             */
/*   Updated: 2024/03/07 16:16:39 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <string.h>
#include <stdio.h>

size_t	newlinech_finder(char *s, size_t n)
{
	size_t	i;

	i = 0;
	if (!s || n == 0)
		return (0);
	while (n > 0)
	{
		if (s[i] == '\n')
			return (i);
		i++;
		n--;
	}
	return (0);
}

// void	ft_bzero(void *s, size_t n)
// {
// 	unsigned char	*str;
// 	size_t			i;

// 	str = (unsigned char *)s;
// 	i = 0;
// 	while (i < n)
// 	{
// 		str[i] = 0;
// 		i++;
// 	}
// }

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
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
	while (i +1< n)
	{
		d[i] = s[i];
		i++;
	}
    if (n != 0)
        d[i] = '\0';
	return (d);
}

char	*ft_strjoin(char *rvalue1, char *buffer1)
{
	char	*joined;
	size_t	rvalue1_len;
	size_t	buffer1_len;

	rvalue1_len = 0;
	buffer1_len = ft_strlen(buffer1);
	if (rvalue1)
		rvalue1_len = ft_strlen(rvalue1);
	else
		rvalue1_len = 0;
	joined = malloc(sizeof(char) * (rvalue1_len + buffer1_len + 1));
	if (!joined)
		return (NULL);
	my_memcpy(joined, rvalue1, rvalue1_len + 1);// +1 or not?
	my_memcpy(joined + rvalue1_len, buffer1, buffer1_len + 1);
	// free((char*)s1); //do i need to cast?
	return (joined);
}

// void	*ft_calloc(size_t count, size_t size)
// {
// 	void	*p;
// 	size_t	total_size;

// 	total_size = count * size;
// 	p = malloc(total_size);
// 	if (p == NULL)
// 		return (NULL);
// 	ft_bzero(p, total_size);
// 	return (p);
// }

void	*ft_memmove(void *dst, void *src, size_t len)
{
	size_t	i;

	i = 0;
	if (len == 0 || dst == src)
		return (dst);
	if (src < dst)
	{
		while (len > 0)
		{
			((unsigned char *)dst)[len - 1] = ((unsigned char *)src)[len - 1];
			len--;
		}
		return (dst);
	}
	else
	{
		while (len > 0)
		{
			*((unsigned char *)dst + i) = *((unsigned char *)src + i);
			i++;
			len--;
		}
	}
	return (dst);
}

char	*ft_strdup(char *s1)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = malloc (ft_strlen(s1) + 1);
	if (!dup)
		return (NULL);
	while (s1[i] != '\0')
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	s1 = NULL;
	return (dup);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	malloc_len;

	i = 0;
	malloc_len = 0;
	if (!s)
		return (NULL);
	if (start > ft_strlen(s) || len == 0 || s[0] == '\0')
	{
		return (ft_strdup(""));
	}
	if (len < ft_strlen(&s[start]))
	{
		malloc_len = len + 1;
	}
	else
	{
		malloc_len = ft_strlen(&s[start]) + 1;
	}
	str = malloc(malloc_len);
	if (!str)
		return (NULL);
	my_memcpy(str, &s[start], malloc_len);
	return (str);
}

/* GNL
	first check if anything is in buf and if yes, check if \n is there, and if yes, strjoin
	it to rvalue, and memmove the rest to start of buf. Then UNTIL \n or EOF is found in
	the buffer (if found, check that there is * management * for another \n in it as well as for 
	content after)- keep reading the file BUFFER_SIZE at a time, strjoining the chunks to rvalue.
	again, if \n found then strjoin part of buffer, memmove the rest and zero out till end. +Errors.
*/


int	buffer_handler(char *buffer, char *rvalue, int *error)
{
	size_t	indexof_first_newline;
	size_t	leftoverlen;
	char	*temp;

	indexof_first_newline = 0;
	leftoverlen = 0;
	temp = NULL;
	if (*buffer)
		indexof_first_newline = newlinech_finder(buffer, BUFFER_SIZE);
	if (indexof_first_newline != 0)
	{
		leftoverlen = ft_strlen(&buffer[indexof_first_newline + 1]);
		temp = ft_substr(buffer, 0, &buffer[indexof_first_newline] - buffer);
		rvalue = ft_strjoin(rvalue, temp);
		ft_memmove(buffer, &buffer[indexof_first_newline +1], leftoverlen);
		bzero(&buffer[leftoverlen], BUFFER_SIZE - leftoverlen);
		//Chek malloc +ret if err + free temp etc. use calloc in place of malloc(if allowed)
		*error = 42;
		return (1);
	}
	else
	{
		rvalue = ft_strjoin(rvalue, buffer);
	}
	return (0);
}

void	freeall(void)
{
	printf("\nFREE ALL\n");
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*rvalue;
	int			is_there_newline;
	int			err;

	rvalue = NULL;
	err = 42;
	is_there_newline = 0;
	while (err != 0 && err != -1 && fd >= 0)
	{
		is_there_newline = buffer_handler(buffer, rvalue, &err);
		if (err != -1 && is_there_newline == 1)
			return (rvalue);
		else if (err != -1)
			err = read(fd, buffer, BUFFER_SIZE);
		else
			{
				printf("\nWHAAT?!\n");
				break ;
			}
	}
	if (err == -1 || err == 0 || fd < 0)
		return (NULL);
	return (rvalue);
}
