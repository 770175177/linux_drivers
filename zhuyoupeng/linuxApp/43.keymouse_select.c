#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int main()
{
	// read mouse
	int fd = -1;
	int ret = -1;
	int flag = -1;
	char buf[100];
	fd_set myset;
	struct timeval tm;

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
	FD_ZERO(&myset);
	FD_SET(0, &myset);
	FD_SET(fd, &myset);
	
	tm.tv_sec = 5;
	tm.tv_usec = 0;

	ret = select(fd+1, &myset, NULL, NULL, &tm);
	if(ret < 0)
	{
		perror("select");
		exit(-1);
	}
	else if(ret == 0)
	{
		printf("timeout\n");
	}
	else
	{
		// get an IO, watch which IO, then handle it
		if(FD_ISSET(0, &myset))
		{
			// keyboard
			memset(buf, 0, sizeof(buf));
			ret = read(0, buf, 5);
			printf("after keyboard, read content: [%s]\n", buf);
		}
		if(FD_ISSET(fd, &myset))
		{
			// mouse
			memset(buf, 0, sizeof(buf));
			ret = read(0, buf, 5);
			printf("after mouse, read content: [%s]\n", buf);
		}
	}

	return 0;
}
