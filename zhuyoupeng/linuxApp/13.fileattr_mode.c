#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define NAME "1.txt"

int main(void)
{
	int ret = -1;
	int result = -1;
	struct stat buf;

	memset(&buf, 0, sizeof(buf));
	ret = stat(NAME, &buf);
	if(ret < 0)
	{
		perror("stat");
		exit(-1);
	}
	result = S_ISREG(buf.st_mode);
	printf("result = %d\n", result);

	result = (buf.st_mode & S_IRWXU) >> 8;
	printf("result = %d\n", result);
	printf("mode = %d\n", buf.st_mode);

	return 0;
}
