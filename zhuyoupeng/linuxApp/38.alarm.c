#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void func(int sig)
{
	printf("alarm func %d\n", sig);
}

int main()
{
	unsigned int ret = -1;

	signal(SIGALRM, func);
	ret = alarm(2);
	printf("ret = %d\n", ret);
	//sleep(3);
	pause();

	return 0;
}
