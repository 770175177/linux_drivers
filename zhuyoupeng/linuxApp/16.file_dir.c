#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	struct dirent *pEnt = NULL;
	DIR *pDir = NULL;
	int cnt = 0;
	
	if(argc != 2)
	{
		printf("usage: %s dirname\n", argv[0]);
		exit(-1);
	}
	pDir = opendir(argv[1]);
	if(NULL == pDir)
	{
		perror("opendir");
		exit(-1);
	}

	while(1)
	{
		pEnt = readdir(pDir);
		if(NULL != pEnt)
		{
			printf("name: [%s], ", pEnt->d_name);
			cnt++;
			if(DT_REG == pEnt->d_type)
			{
				printf("is regular file\n");
			}
			else
			{
				printf("is not regular file\n");
			}
		}
		else
		{
			break;
		}
	}
	printf("total files is %d\n", cnt);

	return 0;
}
