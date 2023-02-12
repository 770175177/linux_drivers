#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define GPIO_READ  "/dev/gpio_read_ctl"

int main(int argc, const char *argv[])
{
	int fd, cmd = 2;
	char *gpio_read = GPIO_READ;
	char *cmd0 = "0";
	char *cmd1 = "1";

	printf("argv[0] is %s, argv[1] is %s\n", argv[0], argv[1]);

	if((fd = open(gpio_read, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("APP open %s failed\n", gpio_read);
	}
	else
	{
		printf("APP open %s success!\n", gpio_read);
	
		if (strcmp(argv[1], cmd0) == 0)
		{
			cmd = 0;
		}
		if (strcmp(argv[1], cmd0) == 0)
		{
			cmd = 1;
		}

		printf("%d io value id %d\n", cmd, ioctl(fd, cmd, 0));
	}

	close(fd);
}
