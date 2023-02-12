#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
	int ret;
	int arg;
	int fd;

	if(argc != 2)
	{
		printf("usage: %s 0|1\n", argv[0]);
		return -1;
	}
	if(fd = open("/dev/max485_ctl_pin", O_RDWR))
	{
		printf("open error\n");
		return -1;
	}
	printf("open ok\n");
	arg = atoi(argv[1]);

	printf("ioctl, arg=%d\n", arg);
	ret = ioctl(fd, arg, arg);
	if(ret < 0)
	{
		printf("ioctl error\n");
		return -1;
	}
	close(fd);	

	return 0;
}
