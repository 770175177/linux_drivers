#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void *func(void *arg);

int main()
{
	int ret = -1;
	char buf[100];
	pthread_t th = -1;

	ret = pthread_create(&th, NULL, func, NULL);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(-1);
	}

	// main thread
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

	return 0;
}

// sub thread
void *func(void *arg)
{
	char buf[100];
	int ret = -1;	

	while(1)
	{
		// keyboard
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, 5);
		printf("after keyboard, read content(%d): [%s]\n", ret, buf);
	}
}
