#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE 128

int main(int argc, const char *argv[])
{
	int ret = -1;
	int sockfd = -1;
	char buf[BUF_SIZE];
	struct sockaddr_in server_addr = {0};

	if(3 != argc)
	{
		printf("usage: %s %s %s\n", argv[0], argv[1], argv[2]);
		exit(-1);
	}
	
	// 1. create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	printf("sockfd = %d\n", sockfd);

	// 2. connect
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	ret = connect(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0)
	{
		perror("connect");
		exit(-1);
	}
	printf("connect success, ret = %d\n", ret);
	
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		printf("send:");
		scanf("%s", buf);
		write(sockfd, buf, sizeof(buf));
		if(!strncasecmp(buf, "quit", 4))
		{
			break;
		}
	}

	return 0;
}
