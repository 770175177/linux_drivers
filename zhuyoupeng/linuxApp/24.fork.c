#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t p1 = -1;

	p1 = fork();
	if(p1 < 0)
	{
		perror("fork");
		exit(-1);
	}
	else if(0 == p1)
	{
		// son
		printf("son, pid = %d\n", getpid());
		printf("son, ppid = %d\n", getppid());
	}
	else
	{
		// parent
		printf("parent, pid = %d\n", getpid());
		printf("parent, p1 = %d\n", p1);
		sleep(1);
	}
	//printf("hello process, pid = %d\n", getpid());

	return 0;
}
