#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define FILENAME "1.txt"

int main(void)
{
	FILE *fp = NULL;
	int len = -1;
	int array[10] = {1, 2, 3, 4, 5};

	fp = fopen(FILENAME, "w+");
	if(NULL == fp)
	{
		perror("fopen");
	}
	printf("fopen success\n");

	len = fwrite(array, sizeof(int), sizeof(array)/sizeof(array[0]), fp);
	printf("len = %d\n", len);

	fclose(fp);

	return 0;
}
