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

/* ========================================================================
	 	newlinech_finder - returns ssize_t index of found \n,
		OR index which matches BUFSIZE if no \n but full buffer found,
		OR -1 if no \n AND not full buffer found
	======================================================================== */
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
	if (i == BUFFER_SIZE) // this step may allow further simplificaiton of buffer_handler()
		return (i);
	return (-1);
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

/*	========================================================================
	- buffer_handler should sort through up to (bytesread/until \0) of it and if needed, store it in rvalue
		and shift its content by index_of_newline or if no \n, shift all until \0. bzero the rest until BUF_SIZE
	- call check for \n char function
	- if bytesread == 0 && *rvalue != \0 //if not at start of function and bytesread is 0
	- returns flag 1 if found \n 	
	- Try refactoring rewriting using only one var, called OFFSET in place of buflen, index_of_nl
		to pass to functions below and call them only once, instead of 3 times like now 
	======================================================================== */
int	buffer_handler(char *buffer, char **rvalue, int *error)
{
	ssize_t	index_of_nl; // do i need to initialize?
	size_t	buflen;

	buflen = 0;
	if (*buffer != 0) //correct syntax for array?
	{
		index_of_nl = newlinech_finder(buffer);
		// if there is a \n in buffer OR the buffer is full even without a \n in it, add to rvalue until \n or buffer end
		if (index_of_nl > -1 && index_of_nl != BUFFER_SIZE)
		{
			*rvalue = ft_strjoin(*rvalue, buffer, index_of_nl + 1);
			ft_memmove(buffer, &buffer[index_of_nl + 1], BUFFER_SIZE - index_of_nl);
			my_bzero(&buffer[BUFFER_SIZE - index_of_nl]);
			return (1);
		}
		else //if \n not found
		{
			buflen = ft_strlen(buffer);
			if (buflen < BUFFER_SIZE)
			{
				*rvalue = ft_strjoin(*rvalue, buffer, buflen);
				if (NULL == *rvalue)
					return(*error = -1);
			// printf("\nrvalue:%s; buffer:%s;\n", *rvalue, buffer); // OOOO=OOOO STATUS OOOO=OOOO
				ft_memmove(buffer, &buffer[buflen], BUFFER_SIZE - buflen);
				my_bzero(&buffer[BUFFER_SIZE - buflen]);
			}
			else
			{
				*rvalue = ft_strjoin(*rvalue, buffer, BUFFER_SIZE);
				my_bzero(buffer);
			}
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
		if (1 == buffer_handler(buffer, &rvalue, &err)) // if buf_handler found a \n
		{
			if (err == -1)
				return (NULL);
			return(rvalue);
		}
		bytesread = read(fd, buffer, BUFFER_SIZE);
	}
	if (rvalue == NULL && (bytesread == 0 || bytesread == -1))
		return (NULL);
	//free() - have it return NULL
	return (rvalue);
}
