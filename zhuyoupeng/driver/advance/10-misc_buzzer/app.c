#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_FILE "/dev/buzzer_ctl"
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

	// ioctl
	ioctl(fd, 1, 2000);
	printf("2000Hz\n");
	sleep(2);
	ioctl(fd, 0, 300);
	sleep(2);
	ioctl(fd, 1, 500);
	printf("500Hz\n");
	sleep(2);
	ioctl(fd, 0, 300);
	

	close(fd);

	return 0;
}
