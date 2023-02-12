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

	close(1);   // close stdout

	fd2 = dup(fd1);  // copy fd1, fd2 = 1,beacuse fd=1 close before,
			 // then fd1 bind to stdout, stdout will to fd1.
	printf("fd2 = %d\n", fd2);
	printf("This is for test\n");

	// close file
	close(fd1);	

	return 0;
}
