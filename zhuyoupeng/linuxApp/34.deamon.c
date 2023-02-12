#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void create_daemon(void);

int main()
{
	int i = 0;
	create_daemon();
	
	while(1)
	{
		printf("I am running %d\n", i++);
		sleep(1);
	}

	return 0;
}

void create_daemon(void)
{
	int ret = -1;
	int i = 0;
	int cnt = -1;
	pid_t pid;

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(-1);
	}
	if(pid > 0)
	{
		// parent
		exit(0);
	}
	// child
	// step1: create new session in order to get out of terminal
	pid = setsid();
	if(pid < 0)
	{
		perror("setsid");
		exit(-1);
	}
	// step2: change workspace to root
	chdir("/");
	// step3: set umask to 0 to get a max file operation permission
	umask(0);
	// step4: close all files description
	cnt = sysconf(_SC_OPEN_MAX);
	for(i = 0; i < cnt; i++)
	{
		close(i);
	}
	// step5: located 0/1/2 to /dev/null
	open("/dev/null", O_RDWR);   // 0
	open("/dev/null", O_RDWR);   // 1
	open("/dev/null", O_RDWR);   // 2
}
