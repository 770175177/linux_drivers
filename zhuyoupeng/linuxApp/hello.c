#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;

	printf("argc = %d\n", argc);
	while(NULL != argv[i])
	{
		printf("argv[%d] = %s\n", i, argv[i]);
		i++;
	}

	return 0;
}
