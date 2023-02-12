#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>

#define DEV_FILE "/dev/fb"
#define BUF_SIZE 100

#define WIDTH		544 
#define HEIGHT		480

#define WHITE		0xffffffff
#define BLACK		0x00000000
#define BLUE		0xffffff00
#define RED		0xffff0000

void draw_back(unsigned int width, unsigned int height, unsigned int color);
unsigned int *pfb = NULL;
void draw_line(unsigned int color);

int main(void)
{
	int ret = -1;
	int fd = -1;
	int i = 0;
	char name[12];
	//char ss = [4];
	unsigned long len;
	struct fb_fix_screeninfo finfo = {0};
	struct fb_var_screeninfo vinfo = {0};

	while(i++<1)
	{
		// step1: open device
		memset(name, 0, sizeof(name));
		//memset(ss, 0, sizeof(ss));
		sprintf(name, "%s%d\0", DEV_FILE, i);
		//strcat(name, itoa(i));
		printf("%s\n", name);
		fd = open(name, O_RDWR);
		if(fd < 0)
		{
			printf("open %s error\n", name);
			exit(-1);
		}
		printf("open %s success\n", name);

		// step2: get hardware device info
		ret = ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
		if(ret < 0)
		{
			perror("ioctl");
			exit(-1);
		}
		//printf("smem_start = 0x%x, smem_len = %u\n", finfo.smem_start, finfo.smem_len);
		printf("xres = %u, yres = %u\n", vinfo.xres, vinfo.yres);
		printf("xres_virtual = %u, yres_virtual = %u\n", vinfo.xres_virtual, vinfo.yres_virtual);
		printf("bpp - %u\n", vinfo.bits_per_pixel);

		// step3: mmap
		len = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
		pfb = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if(NULL == pfb)
		{
			perror("mmap");
			exit(-1);
		}
		printf("pfb = %p\n", pfb);

		draw_back(WIDTH, HEIGHT, WHITE);
		draw_line(RED);
	}

	close(fd);

	return 0;
}

void draw_back(unsigned int width, unsigned int height, unsigned int color)
{
	unsigned int x, y;

	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			*(pfb + y*WIDTH + x) = color;
		}
	}

}

void draw_line(unsigned int color)
{
	unsigned int x, y;

	for(x=50; x<100; x++)
	{
		*(pfb + 100*WIDTH + x) = color;
	}
}
