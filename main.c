#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int	main(void)
{
	int	fd;

	fd = open("test.txt", O_RDONLY);
	printf("result from fd %d:|%s|\n", fd, get_next_line(fd));
	printf("result from fd %d:\n|%s|\n", fd, get_next_line(fd));
	printf("result from fd %d:\n|%s|\n", fd, get_next_line(fd));
	printf("result from fd %d:\n|%s|\n", fd, get_next_line(fd));
	return 0;
}