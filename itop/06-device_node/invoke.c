#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define HELLO_NODE  "/dev/hello_ctl123"

int main()
{
	int fd;
	char *hello_node = HELLO_NODE;

	if((fd = open(hello_node, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("APP open %s failed\n", hello_node);
	}
	else
	{
		printf("APP open %s success\n", hello_node);
		ioctl(fd, 1, 6);
	}

	close(fd);
}
