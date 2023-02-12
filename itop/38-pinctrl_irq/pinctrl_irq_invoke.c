#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int fd;

	if (argc != 2)
	{
		printf("argc is 2: \n\targv1 is device node\n");
	}

	if ((fd = open(argv[1], O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
	{
		printf("open %s failed\n", argv[1]);
		return -1;
	}
	else
	{
		printf("open %s ok\n", argv[1]);
		printf("key value is %d\n", ioctl(fd, 0, 0));
	}

	close(fd);
	return 0;
}
