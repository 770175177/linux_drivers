#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define POLLKEY  "/dev/pollkey"

int main()
{
	int fd;
	unsigned char buffer[2];
	char *pollkey = POLLKEY;

	if((fd = open(pollkey, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("APP open %s failed\n", pollkey);
		return -1;
	}
	else
	{
		printf("APP open %s success\n", pollkey);
	}

	while(1)
	{
		read(fd, buffer, sizeof(buffer));
		if (!buffer[0] || !buffer[1])
		{
			printf("key home value is %d\n", buffer[0]);
			printf("key back value is %d\n", buffer[1]);
		}
	}

	close(fd);
}
