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

	fd = open("a.txt", O_RDWR|O_CREAT|O_EXCL);
	if(fd < 0)
	{
		perror("open");
		_exit(-1);
	}
	else
	{
		printf("open file ok, fd = %d\n", fd);
	}

	// write/read file
	ret = read(fd, buf, 20);
	if(ret < 0)
	{
		printf("read error\n");
		_exit(-1);
	}
	else
	{
		printf("actual read %d bytes\n", ret);
		printf("file contain [%s]\n", buf);
	}

	ret = write(fd, str, strlen(str));
	if(ret < 0)
	{
		printf("write error\n");
		_exit(-1);
	}
	else
	{
		printf("write success, write %d bytes\n", ret);
	}

	// close file
	close(fd);	

	return 0;
}
