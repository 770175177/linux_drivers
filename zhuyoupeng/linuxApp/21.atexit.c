#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void func1(void)
{
	printf("func1\n");
}
void func2(void)
{
	printf("func2\n");
}
void func3(void)
{
	printf("func3\n");
}

int main(void)
{
	printf("hello world\n");

	atexit(func1);
	atexit(func2);
	atexit(func3);

	printf("my name is ...\n");

	//return 0;
	//exit(0);
	_exit(0);
}
