#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define DEVICE_KEY	"/dev/input/event1"
#define DEVICE_MOUSE 	"/dev/input/event4"

int main(void)
{
	int fd = -1, ret = -1;
	int i =  0;
	struct input_event ev;

	// step1: open
	fd = open(DEVICE_KEY, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		exit(-1);
	}
	while(1)
	{
		// step2: read
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd, &ev, sizeof(struct input_event));
		if(ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd);
			exit(-1);
		}
		// step3: param event package
		printf("-----------------%d-----------------\n", i++);
		printf("time: %ld-%ld\n", ev.time.tv_sec, ev.time.tv_usec);
		printf("type: %d\n", ev.type);
		printf("code: %d\n", ev.code);
		printf("value: %d\n\n", ev.value);
	}

	// step4: close
	close(fd);

	return 0;
}
