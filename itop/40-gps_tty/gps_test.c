#include <stdio.h>      /*标准输入输出定义*/  
#include <stdlib.h>     /*标准函数库定义*/  
#include <unistd.h>     /*Unix 标准函数定义*/  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <string.h>  
#include <fcntl.h>  
#include <termios.h>  
  
#define  BUFF_SIZE 512  
#define  MAX_COM_NUM 3  
  
int  SectionID=0,i=0;  
  
 struct data  
{  
    char GPS_time[20];          //UTC时间  
    char GPS_sv;               //使用卫星  
    char GPS_wd[12];           //纬度  
    char GPS_jd[12];           //经度  
    //char GPS_warn;             //定位警告   
    char GPS_speed[5];         //速度  
    char GPS_date[8];          //UTC日期            
          
}GPS_DATA;  
  
  
  
int set_com_config(int fd,int baud_rate,int data_bits,char parity,int stop_bits)  
{  
    struct termios new_cfg,old_cfg;  
    int speed;  
    //保存并测试现有串口参数设置，在这里如果串口号出错，会有相关的出错信息  
      
    if(tcgetattr(fd,&old_cfg)!=0)  
    {  
        perror("tcgetattr");  
        return -1;  
    }  
     tcflush(fd, TCIOFLUSH);  
    new_cfg=old_cfg;  
    cfmakeraw(&new_cfg);//配置为原始模式  
    new_cfg.c_cflag&=~CSIZE;  
  
    //设置波特率  
    switch(baud_rate)  
    {  
        case 2400:  
        {  
            speed = B2400;  
            break;  
        }  
        case 4800:  
        {  
            speed = B4800;  
            break;  
        }  
        case 9600:  
        {  
            speed = B9600;  
            break;  
        }  
        case 19200:  
        {  
            speed = B19200;  
            break;  
        }  
        case 38400:  
        {  
            speed = B38400;  
            break;  
        }  
        case 115200:  
        {  
            speed = B115200;  
            break;   
        }  
          
  
    }  
    cfsetispeed(&new_cfg,speed);  
    cfsetospeed(&new_cfg,speed);  
    //设置数据位  
  
    switch(data_bits)  
    {  
        case 7:  
        {  
            new_cfg.c_cflag|=CS7;  
            break;  
        }  
          
        case 8:  
        {  
            new_cfg.c_cflag|=CS8;  
            break;  
        }  
          
    }  
  
    //设置停止位  
    switch(stop_bits)  
    {  
        case 1:  
        {  
            new_cfg.c_cflag &=~CSTOPB;  
            break;  
        }  
  
        case 2:  
        {  
            new_cfg.c_cflag |=CSTOPB;  
            break;  
        }  
          
          
    }  
  
    //设置奇偶校验位  
    switch(parity)  
    {  
        case 'o':  
        case 'O':  
        {  
            new_cfg.c_cflag|=(PARODD|PARENB);  
            new_cfg.c_iflag|=(INPCK |ISTRIP);  
            break;  
        }  
        case 'e':  
        case 'E':  
        {  
            new_cfg.c_cflag |=PARENB;  
            new_cfg.c_cflag &=~PARODD;  
            new_cfg.c_iflag |=(INPCK | ISTRIP);  
            break;  
        }  
        case 's':  
        case 'S':  
        {  
            new_cfg.c_cflag &=~PARENB;  
            new_cfg.c_cflag &=~CSTOPB;  
            break;  
        }  
          
        case 'n':  
        case 'N':  
        {  
            new_cfg.c_cflag &=~PARENB;  
            new_cfg.c_iflag &=~INPCK;  
            break;  
        }  
          
    }  
  
        new_cfg.c_cc[VTIME] =10;  
    new_cfg.c_cc[VMIN] =5;  
    //处理未接收字符  
     tcflush(fd,TCIFLUSH);  
       
    if((tcsetattr(fd,TCSANOW,&new_cfg))!=0)  
    {  
        perror("tcsetattr");  
        return -1;  
    }  
      
    return 0;  
}  
  
  
//打开串口函数  
int open_port(char* com_port)  
{  
    int fd;  
    char* dev = com_port;  
        //打开串口  
        if((fd=open(dev,O_RDWR|O_NOCTTY|O_NDELAY))<0)  
        {  
            perror("open serial port");  
            return -1;  
        }  
        //恢复串口为堵塞状态  
        if(fcntl(fd,F_SETFL,0) <0 )  
        {  
            perror("fcntl F_SETFL\n");  
            return -1;  
              
        }  
        //测试是否为终端设备  
        if(isatty(STDIN_FILENO)==0)  
        {  
            perror("standard input is not a terminal device");  
        }  
        return fd;  
}  
  
void print_info(void)  
{  
    //打印选择界面，即引导的字符号  
    printf("Now the receive time is %s\n",GPS_DATA.GPS_time);  
    printf("The star is %c 3\n",GPS_DATA.GPS_sv);  
    printf("The earth latitude is :%s\n",GPS_DATA.GPS_wd);  
    printf("The earth longitude is :%s\n",GPS_DATA.GPS_jd);   
    printf("The train speed is:%s km/h\n",GPS_DATA.GPS_speed);  
    printf("The date is:%s\n",GPS_DATA.GPS_date);  
      
}  
  
  
void GPS_resolve_GPRMC(char data)  
{  
//$GPRMC,092427.604,V,4002.1531,N,11618.3097,E,0.000,0.00,280812,,E,N*08  
  
      
    if(data==',')  
    {  
        ++SectionID;  
        i=0;  
    }  
    else  
    {  
        switch(SectionID)  
        {  
            case 1://02:48:13         
                    GPS_DATA.GPS_time[i++]=data;          
                    if(i==2 || i==5)  
                    {         
                        GPS_DATA.GPS_time[i++]=':';       
                    }                 
                    GPS_DATA.GPS_time[8]='\0';  
                break;  
            case 2:  
                if(data=='A')  
                    GPS_DATA.GPS_sv='>';  
                else  
                    GPS_DATA.GPS_sv='<';  
                break;  
            case 3://3158.4608  
                    GPS_DATA.GPS_wd[++i]=data;    
                    GPS_DATA.GPS_wd[12]='\0';                     
                break;  
                  
            case 4:  
                if(data=='N')  
                    GPS_DATA.GPS_wd[0]='N';  
                else if(data=='S')  
                    GPS_DATA.GPS_wd[0]='S';  
              
                break;  
            case 5://11848.3737,E  
                  
                    GPS_DATA.GPS_jd[++i]=data;    
                    GPS_DATA.GPS_jd[12]='\0';  
                break;  
                  
            case 6:  
                if(data=='E')  
                    GPS_DATA.GPS_jd[0]='E';  
                else if(data=='W')  
                    GPS_DATA.GPS_jd[0]='W';  
                  
                break;  
            case 7://10.05  
                    GPS_DATA.GPS_speed[i++]=data;  
                    GPS_DATA.GPS_speed[4]='\0';                       
                break;  
            case 9://15-07-06 -> 06-07-15  
                    GPS_DATA.GPS_date[i++]=data;      
                    if(i==2 || i==5)                          
                    {  
                        GPS_DATA.GPS_date[i++]='-';  
                    }                                 
                    GPS_DATA.GPS_date[8]='\0';                    
                break;  
        }  
    }         
  
}  
  
void read_data(int fd)  
{  
    char buffer[BUFF_SIZE],dest[1024];     
    char array[10]="$GPRMC";  
    int  res,i=0,j=0,k;  
    int data=1,len=0;  
    memset(dest,0,sizeof(dest));  
      
do  
{      
     memset(buffer,0,sizeof(buffer));  
//$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50  
        if(res=read(fd,buffer,1)>0)  
        {         
                //此处源源不断传入参数，一次读到数据可能为（$GPRMC,024），res为读到长度，现在把每一位传入函数处理；     
            strcat(dest,buffer);  
            if(buffer[0]=='\n')  
            {  
                i=0;  
                if(strncmp(dest,array,6)==0)  
                {                 
                        printf("%s",dest);  
                        len=strlen(dest);  
                        for(k=0;k<len;k++)  
                        {  
                            GPS_resolve_GPRMC(dest[k]);  
                        }         
                            SectionID=0;  
                              
                        print_info();  
                }  
                bzero(dest,sizeof(dest));  
            }  
                  
        }  
}while(1);  
    close(fd);  
  
}  
  
int main(int argc,char*argv[])  
{         
        int fd=0;        
        char* HOST_COM_PORT = argv[1];      
	if(2 != argc)
        {
                printf("Usage:  gps_test [uart port]\r\n");
               
                return;
        }
 
        fd=open_port(HOST_COM_PORT);  
        if(fd<0)   
        {  
             perror("open fail!");  
        }  
        printf("open sucess!\n");  
        if((set_com_config(fd,9600,8,'N',1))<0)    
        {  
            perror("set_com_config fail!\n");  
        }  
        printf("The received worlds are:\n");  
        read_data(fd);  
     return 0;  
}  
