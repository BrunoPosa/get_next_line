/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:49:44 by bposa             #+#    #+#             */
/*   Updated: 2024/02/29 20:49:45 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	readline;
	char	buffer[BUFFER_SIZE + 1];
	read(fd, &buffer[0], BUFFER_SIZE);

	return(readline);
}ı