#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

char buf[200] = {0};
sem_t sem;

// sub thread, calc num of chars and print
void *func(void *arg)
{
	// V
	while(1)
	{
		sem_wait(&sem);
		printf("input %ld chars\n", strlen(buf));
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

	sem_init(&sem, 0, 0);

	ret = pthread_create(&th, NULL, func, NULL);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(-1);
	}

	printf("input char, end with enter\n");
	while(scanf("%s", buf))
	{
		// compare, if end exit, else continue
		if(!strncmp(buf, "end", 3))
		{
			printf("process exit...\n");
			break;
		}
		// P
		sem_post(&sem);
	}

	// recycle sub thread
	printf("wait for recycle sub thread\n");
	sem_post(&sem);
	ret = pthread_join(th, NULL);
	if(ret != 0)
	{
		perror("pthread_join");
		exit(-1);
	}
	printf("sub thread recycle ok\n");

	sem_destroy(&sem);

	return 0;
}
