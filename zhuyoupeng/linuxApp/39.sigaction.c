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
	struct sigaction act = {0};

	act.sa_handler = func;
	sigaction(SIGALRM, &act, NULL);

	//signal(SIGALRM, func);
	ret = alarm(5);
	printf("1st, ret = %d\n", ret);
	sleep(3);

	ret = alarm(5);
	printf("2se, ret = %d\n", ret);
	sleep(1);

	ret = alarm(5);
	printf("3td, ret = %d\n", ret);
	sleep(1);

	while(1);

	return 0;
}
