#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define S_ADDR "127.0.0.1"
#define S_PORT 6000
#define BACKLOG 100
#define BUF_SIZE 128

int main()
{
	int ret = -1;
	int clientfd = -1;
	int sockfd = -1;
	socklen_t client_len = 0;
	char buf[BUF_SIZE];
	struct sockaddr_in server_addr = {0};
	struct sockaddr_in client_addr = {0};

	// 1. create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	printf("sockfd = %d\n", sockfd);

	// 2. bind
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(S_PORT);
	server_addr.sin_addr.s_addr = inet_addr(S_ADDR);
	ret = bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(-1);
	}
	printf("bind success, ret = %d\n", ret);

	// 3. listen
	ret = listen(sockfd, BACKLOG);
	if(ret < 0)
	{
		perror("listen");
		exit(-1);
	}
	printf("listen success, ret = %d\n", ret);

	while(1)
	{
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
		if (clientfd < 0)
		{
			printf("error accept\n");
		}
		printf("accpet: %s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		//printf("accept: client fd = %d\n", clientfd);
		while(1)
		{
			memset(buf, 0, sizeof(buf));
			ret = read(clientfd, buf, sizeof(buf));
			printf("recv:%s\n", buf);
			if(!strncasecmp(buf, "quit", 4))
			{
				printf("quit\n");
				break;
			}
		}
	}

	return 0;
}
