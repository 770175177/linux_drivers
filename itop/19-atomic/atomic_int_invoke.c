#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define ATOMIC_INT  "/dev/atomic_int"

int main(int argc, const char *argv[])
{
	int fd;
	char *atomic_int = ATOMIC_INT;

	if((fd = open(atomic_int, O_RDWR|O_NONBLOCK)) < 0)
	{
		printf("%s open %s failed\n", argv[0], atomic_int);
	}
	else
	{
		printf("APP open %s success!\n", argv[0], atomic_int);
	
	}

	while(1);

	close(fd);
}
