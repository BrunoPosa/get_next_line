#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int	main(void)
{
	int	fd;
	int	i = 0;

	fd = open("test.txt", O_RDONLY);
	while (i < 2)
	{
		printf("result from fd %d:\n|%s|\n___________", fd, get_next_line(fd));
		i++;
	}
	close(fd);
	return 0;
}