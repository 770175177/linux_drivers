#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int ret;
	int fd;
	char *hello_node = "/dev/hello_ctl123";

	if((ret = open(hello_node, O_RDWR|O_NDELAY)) < 0)
	{
		printf("App open %s failed\n", hello_node);
	}
	else
	{
		printf("App open %s success\n", hello_node);
		ioctl(fd, 1, 6);
	}

	close(fd);

	return 0;
}
