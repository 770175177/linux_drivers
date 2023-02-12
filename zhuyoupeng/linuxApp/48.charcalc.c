#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(void)
{
	char buf[200] = {0};

	printf("input char, end with enter\n");
	while(scanf("%s", buf))
	{
		// compare, if end exit, else continue
		if(!strncmp(buf, "end", 3))
		{
			printf("process exit...\n");
			exit(0);
		}
		printf("input %ld chars\n", strlen(buf));
		memset(buf, 0, sizeof(buf));
	}

	return 0;
}
