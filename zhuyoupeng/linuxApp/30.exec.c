#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	pid_t pid = -1;
	int status = -1;
	int ret = -1;

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(-1);
	}
	else if(0 == pid)
	{
		// son
		ret = execlp("ls", "ls",  "-l", "-a", NULL);
		//ret = execl("/bin/ls", "ls",  "-l", "-a", NULL);
		//char *const arg[] = {"ls",  "-l", "-a", NULL};
		//execv("/bin/ls", arg);
		//execl("hello", "hello", "world", NULL);	
		if(ret < 0)
		{
			printf("son error, ret = %d\n", ret);
		}

		return 0;
	}
	else
	{
		// parent
		printf("parent, sub process PID = %d\n", pid);
	}

	return 0;
}
