#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void func(int sig)
{
	printf("alarm func %d\n", sig);
}

void my_sleep(unsigned int seconds)
{
	struct sigaction act = {0};
	
	act.sa_handler = func;
	sigaction(SIGALRM, &act, NULL);

	alarm(seconds);
	pause();
}

int main()
{
	printf("before mysleep\n");
	my_sleep(3);
	printf("after mysleep\n");

	return 0;
}
