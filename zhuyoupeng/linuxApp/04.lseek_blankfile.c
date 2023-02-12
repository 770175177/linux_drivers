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

	fd = open("balnk.txt", O_RDWR|O_CREAT);
	if(fd < 0)
	{
		perror("open");
		_exit(-1);
	}
	else
	{
		printf("open file ok, fd = %d\n", fd);
	}

	ret = lseek(fd, 10, SEEK_SET);
	printf("file length: %d Bytes\n", ret);

	ret = write(fd, str, strlen(str));
	if(ret < 0)
	{
		perror("write");
		_exit(-1);
	}
	else
	{
		printf("write ok");
	}

	// close file
	close(fd);	

	return 0;
}
