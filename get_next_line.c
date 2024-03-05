/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:49:44 by bposa             #+#    #+#             */
/*   Updated: 2024/03/05 18:58:45 by bposa            ###   ########.fr       */
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
		if (*s == '\n')
			return (i);
		i++;
		n--;
	}
	return (0);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str;
	size_t			i;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i + 1 < size && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (size != 0)
		dst[i] = '\0';
	return (ft_strlen(src));
}

char	*ft_strjoin(char *s1, char const *s2)
{
	char	*joined;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
	{
		return (NULL);
	}
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joined = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!joined)
	{
		return (NULL);
	}
	ft_strlcpy(joined, s1, s1_len + 1);
	ft_strlcpy(joined + s1_len, s2, s2_len + 1);
	free((char*)s1); //do i need to cast? what happens as this starts bzeroed?
	return (joined);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;
	size_t	total_size;

	total_size = count * size;
	p = malloc(total_size);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, total_size);
	return (p);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
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
	ft_strlcpy(str, &s[start], malloc_len);
	return (str);
}

// void	free_f(void *p)
// {
// 	free(p);
// 	p = NULL; // free or set to NULL the pointer itself?
// 	//consider fflush
// }

	/* 
		until \n or EOF is read, keep reading the file, BUFFER_SIZE at a time,
		storing the read chunks to gotten_next_line.
		if \n found then strjoin part of buffer until \n into gotten_next_line, 
		and memmove part of buf after \n until \0 and zero out rest of buffer.
	*/
char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*gotten_next_line;
	size_t		indexof_nlchar;
	int			err;

	err = 1;
	indexof_nlchar = 0;
	gotten_next_line = NULL;
	while (err != 0  && indexof_nlchar == 0)
	{
		err = read(fd, buffer, BUFFER_SIZE);
		if (err == -1)
		{
			// free_f(gotten_next_line);
			return (NULL);
		}
		indexof_nlchar = newlinech_finder(buffer, BUFFER_SIZE);
		if (indexof_nlchar != 0 || err == 0)
		{
printf("reached here, %ld\n", &buffer[indexof_nlchar] - buffer);
			ft_strjoin(gotten_next_line, ft_substr(buffer, 0, &buffer[indexof_nlchar] - buffer));  // pointer arrythmatic can't be used here like so
			ft_memmove(buffer, &buffer[indexof_nlchar], ft_strlen(&buffer[indexof_nlchar]));
		}
		else
			ft_strjoin(gotten_next_line, buffer);
	}
	return(gotten_next_line);
}
