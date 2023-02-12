#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

int main()
{
	printf("pid = %d\n", getpid());

	openlog("b.out", LOG_PID|LOG_CONS, LOG_USER);

	syslog(LOG_INFO, "this is my log info");

	closelog();

	return 0;
}
