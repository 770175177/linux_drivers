#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>

int main()
{
	int ret = -1;
	int pid =  -1;
	char buf[100];

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(-1);
	}
	else if(pid == 0)
	{
		// child
		int mousefd = open("/dev/input/mouse2", O_RDONLY);
		if(mousefd < 0)
		{
			perror("open");
			exit(-1);
		}

		while(1)
		{
			// mouse
			memset(buf, 0, sizeof(buf));
			ret = read(mousefd, buf, 50);
			printf("after mouse, read content(%d): [%s]\n", ret, buf);
		}	
	}
	else
	{
		// parent
		while(1)
		{
			// keyboard
			memset(buf, 0, sizeof(buf));
			ret = read(0, buf, 5);
			printf("after keyboard, read content(%d): [%s]\n", ret, buf);
		}
	}

	return 0;
}
