#include <stdio.h>
#include<time.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	time_t tNow = 0;
	struct tm tmNow;

	//tNow = time(NULL);
	time(&tNow);
	if(tNow < 0)
	{
		perror("time");
		exit(-1);
	}
	printf("time: %ld\n", tNow);

	printf("ctime: %s\n", ctime(&tNow));

	memset(&tmNow, 0, sizeof(tmNow));
	//gmtime_r(&tNow, &tmNow);
	localtime_r(&tNow, &tmNow);
	printf("%d-%d-%d %d:%d:%d\n", 1900+tmNow.tm_year, 1+tmNow.tm_mon, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	return 0;
}
