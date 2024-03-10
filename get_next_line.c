#include "get_next_line.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


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
	while (i +1< n) //??
	{
		d[i] = s[i];
		i++;
	}
    if (n != 0)
        d[i] = '\0';
	return (d);
}

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

void	my_bzero(char *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		s[i] = 0;
		i++;
	}
}

/*  newlinech_finder
	- check for \n char function
	- return size_t index of the \n if found, OR 0 if not found
*/
ssize_t	newlinech_finder(char *s)
{
	ssize_t	i;

	i = 0;
	if (!s)
		return (-1);
	while (s && s[i] != '\0')
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1); // could returning -i be useful to see when bytesread is < BUFFER_SIZE? beware discerning betw -0 and 0
}

char	*ft_strjoin(char *rvalue1, char *buffer1, size_t buffer_len)
{
	char	*joined;
	size_t	rvalue1_len;

	if (rvalue1)
		rvalue1_len = ft_strlen(rvalue1);
	else
		rvalue1_len = 0;
	joined = malloc(sizeof(char) * (rvalue1_len + buffer_len + 1));
	if (!joined)
		return (NULL);
	my_memcpy(joined, rvalue1, rvalue1_len + 1);// +1 or not?
	my_memcpy(joined + rvalue1_len, buffer1, buffer_len + 1);
	// free((char*)s1); //do i need to cast?
	return (joined);
}

/*	- buffer_handler should sort through up to (bytesread/until \0) of it and if needed, store it in rvalue
		and shift its content by index_of_newline or if no \n, shift all until \0. bzero the rest until BUF_SIZE
	- call check for \n char function
	- if bytesread == 0 && *rvalue != \0 //if not at start of function and bytesread is 0
	- returns flag 1 if found \n 	*/
int	buffer_handler(char *buffer, ssize_t bytesread, char **rvalue, int *error)
{
	ssize_t	index_of_nl; // do i need to initialize?

	if (*buffer != 0) //correct syntax for array?
	{
		index_of_nl = newlinech_finder(buffer);
		if (index_of_nl > -1) // if \n found in buffer ( CAN this BE -i )
		{
			ft_strjoin(*rvalue, buffer, index_of_nl + 1);
			ft_memmove(buffer, &buffer[index_of_nl], index_of_nl + 1);
			my_bzero(&buffer[BUFFER_SIZE - index_of_nl - 1], BUFFER_SIZE - index_of_nl - 1);
			return (1);
		}
		else //if \n not found
		{
			if (bytesread < BUFFER_SIZE)
			{
				if (NULL == ft_strjoin(*rvalue, buffer, bytesread))
					return(*error = -1);
				ft_memmove(buffer, &buffer[bytesread], bytesread + 1);
				my_bzero(&buffer[BUFFER_SIZE - bytesread - 1], BUFFER_SIZE - bytesread - 1);
			}
			else
				ft_strjoin(*rvalue, buffer, BUFFER_SIZE);
		}
	}
	return (0);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*rvalue;
	ssize_t		bytesread;
	int			err;

	rvalue = NULL;
	bytesread = BUFFER_SIZE; // ?
	err = 0;
	while (bytesread != 0 && bytesread != -1 ) // || (bytesread < BUFFER_SIZE && *buffer != 0)
	{
		if (1 == buffer_handler(buffer, bytesread, &rvalue, &err)) // if buf_handler found a \n
		{
			if (err == -1)
				return (NULL);
			return(rvalue);
		}
		// if (bytesread < BUFFER_SIZE)
		// 	return (rvalue);
		bytesread = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytesread == 0 || bytesread == -1)
		return (NULL);
	//free() - have it return NULL
	return (rvalue);
}
