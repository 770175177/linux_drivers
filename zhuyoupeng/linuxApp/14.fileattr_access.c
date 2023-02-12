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

	ret = access(NAME, F_OK);
	if(ret < 0)
	{
		perror("file not exist");
		exit(-1);
	}
	else
	{
		printf("file exist\n");
	}

	ret = access(NAME, R_OK);
	if(ret < 0)
	{
		perror("file not readable");
	}
	else
	{
		printf("file readable\n");
	}

	ret = access(NAME, W_OK);
	if(ret < 0)
	{
		perror("file not writeable");
	}
	else
	{
		printf("file writeable\n");
	}

	ret = access(NAME, X_OK);
	if(ret < 0)
	{
		perror("file not executeable");
	}
	else
	{
		printf("file executeable\n");
	}

	return 0;
}
