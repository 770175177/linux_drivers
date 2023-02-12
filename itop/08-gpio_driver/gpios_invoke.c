#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define HELLO_NODE  "/dev/hello_ctl123"

#define GPIO_NUMS   32

int main(int argc, const char *argv[])
{
	int fd, i, cmd = 2;
	char *hello_node = HELLO_NODE;
	char *cmd0 = "0";
	char *cmd1 = "1";

	printf("argv[0] is %s, argv[1] is %s\n", argv[0], argv[1]);

	if(strcmp(argv[1], cmd0) == 0)
	{
		cmd = 0;
		printf("cmd is 0!\n");
	}

	if(strcmp(argv[1], cmd1) == 0)
	{
		cmd = 1;
		printf("cmd is 1!\n");
	}

	if((fd = open(hello_node, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("APP open %s failed\n", hello_node);
	}
	else
	{
		printf("APP open %s success\n", hello_node);
		for(i = 0; i < GPIO_NUMS; i++)
		{
			ioctl(fd, cmd, i);
			printf("APP ioctl %s, cmd is %d, i is %d\n", hello_node, cmd, i);
		}
	}

	close(fd);
}
