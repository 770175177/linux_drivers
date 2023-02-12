#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUF 100

int main(int argc, char *argv[])
{
	// open file
	int fd = -1;
	char buf[MAX_BUF] = {0};
	int ret = -1;
	char *str = "I love linux";

	if(argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
		_exit(-1);
	}

	fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		_exit(-1);
	}
	else
	{
		printf("open file ok, fd = %d\n", fd);
	}

	ret = lseek(fd, 0, SEEK_END);
	printf("file length: %d Bytes\n", ret);

	// close file
	close(fd);	

	return 0;
}
