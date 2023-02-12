#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUF 100

#define FILENAME    "1.txt"

int main(int argc, char *argv[])
{
	// open file
	int fd1 = -1;
	int fd2 = -1;
	char buf[MAX_BUF] = {0};
	int ret = -1;
	char *str = "I love linux";

	fd1 = open(FILENAME, O_RDWR|O_CREAT|O_TRUNC, 0644);
	if(fd1 < 0)
	{
		perror("open");
		_exit(-1);
	}
	else
	{
		printf("open file ok, fd = %d\n", fd1);
	}

	fd2 = fcntl(fd1, F_DUPFD, 6);

	printf("fd2 = %d\n", fd2);

	// close file
	close(fd1);	

	return 0;
}
