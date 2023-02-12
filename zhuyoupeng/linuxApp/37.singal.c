#include <stdio.h>
#include <signal.h>

void func(int sig)
{
	if(SIGINT != sig)
		return ;
	printf("func for signal: %d\n", sig);
}

int main()
{
	//signal(SIGINT, func);
	//signal(SIGINT, SIG_DFL);
	signal(SIGINT, SIG_IGN);

	printf("before while(1)\n");
	while(1);	
	printf("after while(1)\n");

	return 0;
}
