#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define FILENAME "1.txt"

int main(void)
{
	FILE *fp = NULL;
	int len = -1;
	char buf[100] = {0};

	fp = fopen(FILENAME, "r+");
	if(NULL == fp)
	{
		perror("fopen");
		exit(-1);
	}
	printf("fopen success\n");
	memset(buf, 0, sizeof(buf));
	len = fread(buf, sizeof(char), 10, fp);
	printf("buf is %s\n", buf);
	printf("len = %d\n", len);

	fclose(fp);

	return 0;
}
