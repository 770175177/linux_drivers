#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_FILE "/dev/test"
#define BUF_SIZE 100

int main(void)
{
	int i = 5;
	int fd = -1;
	char buf[BUF_SIZE];

	fd = open(DEV_FILE, O_RDWR);
	if(fd < 0)
	{
		printf("open %s error\n", DEV_FILE);
		exit(-1);
	}
	printf("open %s success\n", DEV_FILE);

	// write or read
	write(fd, "hello world", 11);
	read(fd, buf, BUF_SIZE);
	printf("read content: %s\n", buf);
	while(i--)
	{
		write(fd, "on", 11);
		sleep(i%2+1);
		write(fd, "off", 11);
		sleep(i%2+1);
	}

	close(fd);

	return 0;
}
