/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:34:03 by bposa             #+#    #+#             */
/*   Updated: 2024/03/14 15:57:26 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*my_mmov(void *dst, void *src, size_t len)
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

char	*my_strjoin(char *rvalue1, char *buffer1, size_t buffer_len)
{
	char	*joined;
	size_t	rvalue1_len;

	if (rvalue1)
		rvalue1_len = my_strlen(rvalue1);
	else
		rvalue1_len = 0;
	joined = malloc(sizeof(char) * (rvalue1_len + buffer_len + 1));
	if (!joined)
	{
		if (rvalue1)
		{
			free(rvalue1);
			rvalue1 = NULL;
		}
		return (NULL);
	}
	joined[rvalue1_len + buffer_len] = 0;
	my_bzero(joined);
	my_memcpy(joined, rvalue1, rvalue1_len + 1);
	my_memcpy(joined + rvalue1_len, buffer1, buffer_len + 1);
	if (rvalue1)
		free(rvalue1);
	return (joined);
}

/* 	======================================================================
	shift_count_setter - returns ssize_t index of whichever comes first,
	\n (puts newline flag up) or \0. Used for shifting the buffer by index.
	====================================================================== */

ssize_t	shift_count_setter(char *buffer, int *newline)
{
	ssize_t	i;

	i = 0;
	*newline = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '\n')
		{
			*newline = 1;
			return (i);
		}
		i++;
	}
	return (i);
}

/*	======================================================================
		buffer_handler stores part of buffer until \n or \0 into rvalue,
		shifts its content by shift_count, and bzero-es the rest of buffer.
		It returns 1 if found \n or 0 if not, and -1 on error
	====================================================================== */

int	buffer_handler(char *buffer, char **rvalue, int *error)
{
	ssize_t	shift_count;
	int		newline;

	newline = 0;
	if (*buffer == 0)
		return (0);
	shift_count = shift_count_setter(buffer, &newline);
	*rvalue = my_strjoin(*rvalue, buffer, shift_count + newline);
	if (*rvalue == NULL)
		return (*error = -1);
	my_mmov(buffer, &buffer[shift_count + newline], BUFFER_SIZE - shift_count);
	my_bzero(&buffer[BUFFER_SIZE - shift_count]);
	return (newline);
}

/*	======================================================================
		get_next_line returns rvalue if found \n or if at the end of file
		without errors.	On error, it returns NULL.
	====================================================================== */

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*rvalue;
	int			err;

	rvalue = NULL;
	err = 42;
	while (err != 0 && err != -1)
	{
		if (buffer_handler(buffer, &rvalue, &err) == 1)
			return (rvalue);
		if (err == -1)
		{
			if (rvalue)
				free(rvalue);
			my_bzero(buffer);
			return (NULL);
		}
		err = read(fd, buffer, BUFFER_SIZE);
	}
	if (err == -1 && rvalue)
		return (free(rvalue), NULL);
	if (rvalue == NULL && (err == 0 || err == -1))
		return (NULL);
	return (rvalue);
}
