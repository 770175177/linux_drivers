/*
 * FILE: i2c.c
 * ������������/����
 */
#include <stdio.h>
#include "s3c24xx.h"
#include "i2c.h"

void Delay(int time);

#define WRDATA      (1)
#define RDDATA      (2)

typedef struct tI2C {
    unsigned char *pData;   /* ���ݻ����� */
    volatile int DataCount; /* �ȴ���������ݳ��� */
    volatile int Status;    /* ״̬ */
    volatile int Mode;      /* ģʽ����/д */
    volatile int Pt;        /* pData�д��������ݵ�λ�� */
}tS3C24xx_I2C, *ptS3C24xx_I2C;

static tS3C24xx_I2C g_tS3C24xx_I2C;

/*
 * I2C��ʼ��
 */
void i2c_init(void)
{
    GPEUP  |= 0xc000;       // ��ֹ�ڲ�����
    GPECON |= 0xa0000000;   // ѡ�����Ź��ܣ�GPE15:IICSDA, GPE14:IICSCL

    INTMSK &= ~(BIT_IIC);

    /* bit[7] = 1, ʹ��ACK
     * bit[6] = 0, IICCLK = PCLK/16
     * bit[5] = 1, ʹ���ж�
     * bit[3:0] = 0xf, Tx clock = IICCLK/16
     * PCLK = 50MHz, IICCLK = 3.125MHz, Tx Clock = 0.195MHz
     */
    IICCON = (1<<7) | (0<<6) | (1<<5) | (0xf);  // 0xaf

    IICADD  = 0x10;     // S3C24xx slave address = [7:1]
    IICSTAT = 0x10;     // I2C�������ʹ��(Rx/Tx)
}

/*
 * ��������
 * slvAddr : �ӻ���ַ��buf : ���ݴ�ŵĻ�������len : ���ݳ��� 
 */
void i2c_write(unsigned int slvAddr, unsigned char *buf, int len)
{
    g_tS3C24xx_I2C.Mode = WRDATA;   // д����
    g_tS3C24xx_I2C.Pt   = 0;        // ����ֵ��ʼΪ0
    g_tS3C24xx_I2C.pData = buf;     // ���滺������ַ
    g_tS3C24xx_I2C.DataCount = len; // ���䳤��
    
    IICDS   = slvAddr;
    IICSTAT = 0xf0;         // �������ͣ�����
    
    /* �ȴ�ֱ�����ݴ������ */    
    while (g_tS3C24xx_I2C.DataCount != -1);
}
        
/*
 * ��������
 * slvAddr : �ӻ���ַ��buf : ���ݴ�ŵĻ�������len : ���ݳ��� 
 */
void i2c_read(unsigned int slvAddr, unsigned char *buf, int len)
{
    g_tS3C24xx_I2C.Mode = RDDATA;   // ������
    g_tS3C24xx_I2C.Pt   = -1;       // ����ֵ��ʼ��Ϊ-1����ʾ��1���ж�ʱ����������(��ַ�ж�)
    g_tS3C24xx_I2C.pData = buf;     // ���滺������ַ
    g_tS3C24xx_I2C.DataCount = len; // ���䳤��
    
    IICDS        = slvAddr;
    IICSTAT      = 0xb0;    // �������գ�����
    
    /* �ȴ�ֱ�����ݴ������ */    
    while (g_tS3C24xx_I2C.DataCount != 0);
}

/*
 * I2C�жϷ������
 * ����ʣ������ݳ���ѡ�����������߽���
 */
void I2CIntHandle(void)
{
    unsigned int iicSt,i;

    // ���ж�
    SRCPND = BIT_IIC;
    INTPND = BIT_IIC;
    
    iicSt  = IICSTAT; 

    if(iicSt & 0x8){ printf("Bus arbitration failed\n\r"); }

    switch (g_tS3C24xx_I2C.Mode)
    {    
        case WRDATA:
        {
            if((g_tS3C24xx_I2C.DataCount--) == 0)
            {
                // �������������ָ�I2C����������P�ź�
                IICSTAT = 0xd0;
                IICCON  = 0xaf;
                Delay(10000);  // �ȴ�һ��ʱ���Ա�P�ź��Ѿ�����
                break;    
            }

            IICDS = g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++];
            
            // ������д��IICDS����Ҫһ��ʱ����ܳ�����SDA����
            for (i = 0; i < 10; i++);   

            IICCON = 0xaf;      // �ָ�I2C����
            break;
        }

        case RDDATA:
        {
            if (g_tS3C24xx_I2C.Pt == -1)
            {
                // ����ж��Ƿ���I2C�豸��ַ�����ģ�û������
                // ֻ����һ������ʱ����Ҫ����ACK�ź�
                g_tS3C24xx_I2C.Pt = 0;
                if(g_tS3C24xx_I2C.DataCount == 1)
                   IICCON = 0x2f;   // �ָ�I2C���䣬��ʼ�������ݣ����յ�����ʱ������ACK
                else 
                   IICCON = 0xaf;   // �ָ�I2C���䣬��ʼ��������
                break;
            }

			g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++] = IICDS;
			g_tS3C24xx_I2C.DataCount--;
            
            if (g_tS3C24xx_I2C.DataCount == 0)
            {

                // �������лָ�I2C����������P�ź�
                IICSTAT = 0x90;
                IICCON  = 0xaf;
                Delay(10000);  // �ȴ�һ��ʱ���Ա�P�ź��Ѿ�����
                break;    
            }      
			else
			{           
	           // �������һ������ʱ����Ҫ����ACK�ź�
	           if(g_tS3C24xx_I2C.DataCount == 1)
	               IICCON = 0x2f;   // �ָ�I2C���䣬���յ���һ����ʱ��ACK
	           else 
	               IICCON = 0xaf;   // �ָ�I2C���䣬���յ���һ����ʱ����ACK
			}
           break;
        }
       
        default:
            break;      
    }
}

/*
 * ��ʱ����
 */
void Delay(int time)
{
    for (; time > 0; time--);
}

