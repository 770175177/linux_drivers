#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	char buf[512] = {0};

	if(argc != 2)
	{
		printf("usage: %s -v|-d\n", argv[0]);
		exit(-1);
	}

	if(!strcmp(argv[1], "-v"))
	{
		fd = open("/proc/version", O_RDONLY);
		if(fd < 0)
		{
			perror("open /proc/version");
			exit(-1);
		}
		read(fd, buf, sizeof(buf));
		printf("result: %s\n", buf);
	}
	else if (!strcmp(argv[1], "-d"))
	{
		fd = open("/proc/devices", O_RDONLY);
		if(fd < 0)
		{
			perror("open /proc/devices");
			exit(-1);
		}
		read(fd, buf, sizeof(buf));
		printf("result: %s\n", buf);
	}

	return 0;
}
