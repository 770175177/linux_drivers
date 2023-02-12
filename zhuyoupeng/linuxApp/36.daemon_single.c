#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILE_D "/var/topeet_test_single"

void create_daemon(void);
void delete_file(void);

int main()
{
	int i = 0;
	int fd = -1;

	fd = open(FILE_D, O_RDWR|O_TRUNC|O_CREAT|O_EXCL, 0644);
	if(fd < 0)
	{
		if (EEXIST == errno)
		{
			printf("process exist, do not create\n");
			exit(-1);
		}
	}
	printf("open file success\n");
	
	atexit(delete_file);

	//create_daemon();
	while(i < 10)
	{
		printf("I am running %d\n", i++);
		sleep(1);
	}
	close(fd);

	return 0;
}

void delete_file()
{
	printf("delete %s\n", FILE_D);
	remove(FILE_D);
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
