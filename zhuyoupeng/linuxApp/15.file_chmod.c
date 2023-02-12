#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int ret = -1;

	if(argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
		exit(-1);
	}

	ret = chmod(argv[1], S_IRUSR| S_IWUSR| S_IXUSR);
	if(ret < 0)
	{
		perror("chmod");
		exit(-1);
	}
	printf("chmod ok\n");

	return 0;
}
