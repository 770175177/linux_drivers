#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEV_FILE "/dev/test"

int main(void)
{
	int fd = -1;

	fd = open(DEV_FILE, O_RDWR);
	if(fd < 0)
	{
		printf("open %s error\n", DEV_FILE);
		exit(-1);
	}
	printf("open %s success\n", DEV_FILE);

	// write or read

	close(fd);

	return 0;
}
