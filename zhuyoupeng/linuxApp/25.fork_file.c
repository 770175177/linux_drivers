#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	pid_t pid = -1;
	int fd = -1;

	fd = open("1.txt", O_RDWR|O_TRUNC);
	if(fd < 0)
	{
		perror("open");
		exit(-1);
	}

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(-1);
	}
	else if(0 == pid)
	{
		// son
		//sleep(1);
		printf("child\n");
		write(fd, "world", 5);
	}
	else
	{
		// parent
		printf("parent\n");
		write(fd, "hello", 5);
	}
	close(fd);

	return 0;
}
