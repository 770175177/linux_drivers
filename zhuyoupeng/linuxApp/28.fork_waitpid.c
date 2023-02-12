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
		printf("child, PID = %d\n", getpid());
		return 2;
	}
	else
	{
		// parent
		printf("parent\n");
		//ret = waitpid(-1, &status, 0);
		//ret = waitpid(pid, &status, 0);
		ret = waitpid(pid, &status, WNOHANG);
		printf("parent, recycle child process, child PID = %d, status = %d\n", ret, WEXITSTATUS(status));
	}

	return 0;
}
