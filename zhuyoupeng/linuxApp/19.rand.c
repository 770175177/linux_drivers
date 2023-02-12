#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i = 0, val = 0;

	printf("Rand_max = %d\n", RAND_MAX);
	
	srand(time(NULL));
	for(i = 0; i < 6; i++)
	{
		val = rand();
		printf("%d ", val%6);
	}
	printf("\n");

	return 0;
}
