#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	// read mouse
	int fd = -1;
	char buf[100];

	fd = open("/dev/input/mouse2", O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		exit(-1);
	}

	memset(buf, 0, sizeof(buf));
	printf("before read mouse\n");
	read(fd, buf, 50);
	printf("after mouse, read content: [%s]\n", buf);

	/*
	memset(buf, 0, sizeof(buf));
	printf("before read keyboard\n");
	read(0, buf, 2);
	printf("after keyboard, read content: [%s]\n", buf);
	*/	

	return 0;
}

/*
int main()
{
	// read keyboard
	char buf[100];

	memset(buf, 0, sizeof(buf));
	printf("before read keyboard\n");
	read(0, buf, 2);
	printf("after keyboard, read content: [%s]\n", buf);

	return 0;
}*/
