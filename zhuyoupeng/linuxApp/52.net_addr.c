#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPADDR "192.168.0.102"

int main()
{
#if 1
	struct in_addr addr = {0};
	const char *ret = NULL;
	char buf[50] = {0};

	addr.s_addr = 0x6601a8c0;

	ret = inet_ntop(AF_INET, &addr, buf, sizeof(buf));
	if (NULL == ret)
	{
		printf("inet_ntop error\n");
		exit(-1);
	}
	printf("addr = %s\n", ret);
#endif
#if 0
	struct in_addr addr = {0};
	int ret = -1;

	ret = inet_pton(AF_INET, IPADDR, &addr);
	if (1 != ret)
	{
		printf("inet_pton error\n");
		exit(-1);
	}
	printf("addr = 0x%x\n", addr.s_addr);
#endif
#if 0
	in_addr_t addr = 0;

	//addr = inet_addr(IPADDR);
	// addr(0x) = 0x66 01 a8  c0
	// addr(ocd)  102  0  168 192

	printf("addr = 0x%x\n", addr);
#endif

	return 0;
}
