#include <stdio.h>
#include<time.h>
#include<sys/time.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int ret;
	time_t tNow = 0;
	struct tm tmNow;
	char buf[100];
	struct timeval tv = {0};
	struct timezone tz = {0};

	//tNow = time(NULL);
	time(&tNow);
	if(tNow < 0)
	{
		perror("time");
		exit(-1);
	}
	printf("time: %ld\n", tNow);

	printf("ctime: %s\n", ctime(&tNow));

	// gmtime & localtime
	memset(&tmNow, 0, sizeof(tmNow));
	//gmtime_r(&tNow, &tmNow);
	localtime_r(&tNow, &tmNow);
	printf("localtime: %d-%d-%d %d:%d:%d\n", 1900+tmNow.tm_year, 1+tmNow.tm_mon, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	// strftime
	memset(&buf, 0, sizeof(buf));
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmNow);
	printf("strftime: %s\n", buf);

	// gettimeofday
	ret = gettimeofday(&tv, &tz);
	if(ret < 0)
	{
		perror("gettimeofday");
		exit(-1);
	}
	printf("second: %ld\n", tv.tv_sec);
	printf("timezone: %d\n", tz.tz_minuteswest);
	
	return 0;
}
