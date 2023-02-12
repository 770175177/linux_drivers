#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define TEST_BIT  "/dev/atomic_bit"

int main(int argc, const char *argv[])
{
	int fd;
	char *test_bit = TEST_BIT;

	if((fd = open(test_bit, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("%s open %s failed\n", argv[0], test_bit);
	}
	else
	{
		printf("APP open %s success!\n", argv[0], test_bit);
	
	}

	while(1);

	close(fd);
}
