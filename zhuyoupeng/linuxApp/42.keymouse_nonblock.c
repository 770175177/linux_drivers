#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#if 1
int main()
{
	// read mouse
	int fd = -1;
	int ret = -1;
	int flag = -1;
	char buf[100];

	// fd=0(stdin) set nonblock IO
	flag = fcntl(0, F_GETFL);   // get raw flag
	flag |= O_NONBLOCK;          // and nonblock attr
	fcntl(0, F_SETFL, flag);     // set flag
	// then, 0 change to nonblock IO

	fd = open("/dev/input/mouse2", O_RDONLY|O_NONBLOCK);
	if(fd < 0)
	{
		perror("open");
		exit(-1);
	}

	while(1)
	{
		memset(buf, 0, sizeof(buf));
//		printf("before read mouse\n");
		ret = read(fd, buf, 50);
		if(ret > 0)
		{
			printf("after mouse, read content: [%s]\n", buf);
		}

		memset(buf, 0, sizeof(buf));
//		printf("before read keyboard\n");
		ret = read(0, buf, 5);
		if(ret > 0)
		{
			printf("after keyboard, read content: [%s]\n", buf);
		}
	}

	return 0;
}
#endif

#if 0
int main()
{
	// read keyboard
	char buf[100];
	int flag = -1;

	// fd=0(stdin) set nonblock IO
	flag = fcntl(0, F_GETFL);   // get raw flag
	flag |= O_NONBLOCK;          // and nonblock attr
	fcntl(0, F_SETFL, flag);     // set flag
	// then, 0 change to nonblock IO

	memset(buf, 0, sizeof(buf));
	printf("before read keyboard\n");
	read(0, buf, 2);
	printf("after keyboard, read content: [%s]\n", buf);

	return 0;
}
#endif
