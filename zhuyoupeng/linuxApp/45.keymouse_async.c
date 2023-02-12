#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>

int mousefd = -1;

// async func, handler async events
void func(int sig)
{
	char buf[100] = {0};
	if(sig != SIGIO)
		return ;
	// mouse
	memset(buf, 0, sizeof(buf));
	read(mousefd, buf, 50);
	printf("after mouse, read content: [%s]\n", buf);
	
}

int main()
{
	// read mouse
	int ret = -1;
	int flag = -1;
	char buf[100];

	mousefd = open("/dev/input/mouse2", O_RDONLY);
	if(mousefd < 0)
	{
		perror("open");
		exit(-1);
	}

	// 1.register mousefd async IO notice
	flag = fcntl(mousefd, F_GETFL);
	flag |= O_ASYNC;
	fcntl(mousefd, F_SETFL, flag);
	// 2.set async event receiver to current process
	fcntl(mousefd, F_SETOWN, getpid());
	// 3.register current process SIGIO capture function
	signal(SIGIO, func);

	while(1)
	{
		// keyboard
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, 5);
		printf("after keyboard, read content: [%s]\n", buf);
	}

	return 0;
}
