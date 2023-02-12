#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define HELLO_NODE0  "/dev/chardevnode0"
#define HELLO_NODE1  "/dev/chardevnode1"

int main()
{
	int fd;
	char *hello_node0 = HELLO_NODE0;
	char *hello_node1 = HELLO_NODE1;

	if((fd = open(hello_node0, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("APP open %s failed!\n", hello_node0);
	}
	else
	{
		printf("APP open %s success!\n", hello_node0);
		ioctl(fd, 1, 6);
	}

	if((fd = open(hello_node1, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("APP open %s failed!\n", hello_node1);
	}
	else
	{
		printf("APP open %s success!\n", hello_node1);
		ioctl(fd, 1, 6);
	}

	close(fd);
}
