#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	get_next_line(open("test.txt", O_RDONLY));
	return 0;
}