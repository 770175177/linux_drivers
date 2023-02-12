#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

char buf[200] = {0};
pthread_mutex_t mutex;

// sub thread, calc num of chars and print
void *func(void *arg)
{
	// V
	while(1)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		printf("input %ld chars\n", strlen(buf));
		pthread_mutex_unlock(&mutex);
		if(!strncmp(buf, "end", 3))
		{
			printf("exit sub thread...\n");
			break;
		}
		memset(buf, 0, sizeof(buf));
	}

	pthread_exit(NULL);
}

int main(void)
{
	int ret = -1;
	pthread_t th = -1;

	pthread_mutex_init(&mutex, NULL);

	ret = pthread_create(&th, NULL, func, NULL);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(-1);
	}

	printf("input char, end with enter\n");
	while(1)
	{
		pthread_mutex_lock(&mutex);
		scanf("%s", buf);
		pthread_mutex_unlock(&mutex);
		sleep(1);
		// compare, if end exit, else continue
		if(!strncmp(buf, "end", 3))
		{
			printf("process exit...\n");
			break;
		}
	}

	// recycle sub thread
	printf("wait for recycle sub thread\n");
	ret = pthread_join(th, NULL);
	if(ret != 0)
	{
		perror("pthread_join");
		exit(-1);
	}
	printf("sub thread recycle ok\n");

	pthread_mutex_destroy(&mutex);

	return 0;
}
