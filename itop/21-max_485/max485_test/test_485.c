//#include <stdio.h>
#include <unistd.h>
//#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define MAX485_CONTROL

//#include "uart.c"
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if  ( tcgetattr( fd,&oldtio)  !=  0) { 
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}

	printf("Baund Rate: %d\n", nSpeed);

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	case 921600:
		printf("Rate:921600\n");
		cfsetispeed(&newtio, B921600);
                cfsetospeed(&newtio, B921600);
                break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
	newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
//	printf("set done!\n\r");
	return 0;
}

int prepare_to_send(int fd)
{
	int ret;

	ret = ioctl(fd, 1, 0);
	if(ret<0)
	{
		printf("max485 set ctl to high failed!\r\n");

		return -1;
	}
	else
	{
		return 0;
	}
}

int prepare_to_recv(int fd)
{
	int ret;

	ret = ioctl(fd, 0, 0);
	if(ret<0)
	{
		printf("max485 set ctl to low failed!\r\n");
		
		return -1;
	}
	else
	{
		return 0;
	}
}

void main(int argc, char* argv[])
{
	unsigned char ucTmp;
	int fd1,fd2,nset1,nset2,nread;

	char buf[100];
	//char buf1[1];

	//char *buff = "Hello\n\r";

	int i = 0;

	char *max485_ctl = "/dev/max485_ctl_pin";

	if(3 != argc)
    	{
		printf("Usage:	test_485 [uart port] [type]\r\n");
		printf("		type: 0--recv, 1--send\r\n");
        	
		return;
	}

	fd1 = open(argv[1], O_RDWR);
	if (fd1 == -1)
	{
		printf("Open %s faild\n", argv[1]);
		exit(1);
	}

	nset1 = set_opt(fd1, 9600, 8, 'N', 1);
	if (nset2 == -1)
	{
		printf("Set uart faild\n");
		exit(1);
	}

#ifdef MAX485_CONTROL
	if((fd2=open(max485_ctl, O_RDWR|O_NOCTTY|O_NDELAY))<0)
	{
		printf("Open %s faild\n", max485_ctl);
		close(fd1);
		
		exit(1);
	}
#endif
	
	if(0 == atoi(argv[2]))	//recv
	{
#ifdef MAX485_CONTROL
		prepare_to_recv(fd2);
#endif
		while(1)
		{	
			
			nread = read(fd1, buf, 100);
			if (nread > 0)
			{
				for(i=0; i<nread; i++)
				{
					printf("%c", buf[i]);
			
					if(buf[i] == 'q')
						//break;
						goto exit;
				}
			}
			//if(nread)
			//{
			//	printf("\r\n");
			//}
			sleep(1);
		}
	}
	else	//send 
	{
#ifdef MAX485_CONTROL
		prepare_to_send(fd2);
#endif
		while(1)
		{
			printf("Send data, time:%d\r\n", i);
			sprintf(buf, "iTOP-4412: max485 test app(times:%d)\r\n", i++);
			//nread = write(fd1, "iTOP-4412: max485 test app\r\n", strlen("iTOP-4412: max485 test app\r\n"));
			nread = write(fd1, buf, strlen(buf));
			sleep(1);
#if 0
			nread = read(fd1, buf, 100);
                        if (nread > 0)
                        {
                                for(i=0; i<nread; i++)
                                {
                                        printf("%c", buf[i]);

                                        if(buf[i] == 'q')
                                                //break;
                                                goto exit;
                                }
                        }
                        if(nread)
                        {
                                printf("\r\n");
                        }
#endif
		}
	}
exit:
	close(fd1);

        return;
}

