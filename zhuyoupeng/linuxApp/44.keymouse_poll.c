#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main()
{
	// read mouse
	int fd = -1;
	int ret = -1;
	int flag = -1;
	char buf[100];
	struct pollfd myfds[2] = {0};

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
	// now, there are two fd, mouese and stdin(0)
	// init pollfd
	myfds[0].fd = 0;          // keyboard
	myfds[0].events = POLLIN; // wait read
	myfds[1].fd = fd;         // mouse
	myfds[1].events = POLLIN; // wait read


	ret = poll(myfds, fd+1, 10000);
	if(ret < 0)
	{
		perror("poll");
		exit(-1);
	}
	else if(ret == 0)
	{
		printf("timeout\n");
	}
	else
	{
		// get an IO, watch which IO, then handle it
		if(myfds[0].events == myfds[0].revents)
		{
			// keyboard
			memset(buf, 0, sizeof(buf));
			ret = read(0, buf, 5);
			printf("after keyboard, read content: [%s]\n", buf);
		}
		if(myfds[1].events == myfds[1].revents)
		{
			// mouse
			memset(buf, 0, sizeof(buf));
			ret = read(0, buf, 5);
			printf("after mouse, read content: [%s]\n", buf);
		}
	}

	return 0;
}
