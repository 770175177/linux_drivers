#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	pid_t p1 = -1, p2 = -1;

	printf("hello\n");
	p1 = getpid();
	printf("pid = %d\n", p1);
	p2 = getppid();
	printf("parent id = %d\n", p2);

	return 0;
}
