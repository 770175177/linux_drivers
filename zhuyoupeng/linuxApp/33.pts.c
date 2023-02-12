#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = -1;
	int ret = -1;
	char buf[] = "pwd\n";

	fd = open("/dev/pts/1", O_RDWR);
	if (fd < 0)
	{
		perror("open");
		exit(-1);
	}
	ret = write(fd, buf, sizeof(buf));
	if (ret < 0)
	{
		perror("write");
		exit(-1);
	}
	printf("write to TTY1, len = %d\n", ret);

	close(fd);

	return 0;
}
