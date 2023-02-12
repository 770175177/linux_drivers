#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int fd;

	if (argc != 4)
	{
		printf("argc is 4: \n\targv1 is device node, \n\targv2 is cmd, \n\targv3 is lednum");
	}

	if ((fd = open(argv[1], O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
	{
		printf("open %s failed\n", argv[1]);
		return -1;
	}
	else
	{
		printf("open %s ok\n", argv[1]);
		ioctl(fd, atoi(argv[2]), atoi(argv[3]));
	}

	close(fd);
	return 0;
}
