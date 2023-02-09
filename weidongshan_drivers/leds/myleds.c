#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#define DEVICE_NAME     "leds"  /* ����ģʽ��ִ�С�cat /proc/devices����������豸���� */
#define LED_MAJOR       231     /* ���豸�� */


static struct class *leds_class;
static struct class_device	*leds_class_devs[4];


/* bit0<=>D10, 0:��, 1:�� 
 *  bit1<=>D11, 0:��, 1:�� 
 *  bit2<=>D12, 0:��, 1:�� 
 */ 
static char leds_status = 0x0;  
static DECLARE_MUTEX(leds_lock); // ���帳ֵ

//static int minor;
static unsigned long gpio_va;

#define GPIO_OFT(x) ((x) - 0x56000000)
#define GPFCON  (*(volatile unsigned long *)(gpio_va + GPIO_OFT(0x56000050)))
#define GPFDAT  (*(volatile unsigned long *)(gpio_va + GPIO_OFT(0x56000054)))


/* Ӧ�ó�����豸�ļ�/dev/ledsִ��open(...)ʱ��
 * �ͻ����s3c24xx_leds_open����
 */
static int s3c24xx_leds_open(struct inode *inode, struct file *file)
{
	int minor = MINOR(inode->i_rdev); //MINOR(inode->i_cdev);

	switch(minor)
	{
        case 0: /* /dev/leds */
        {
            // ����3����Ϊ���
            //s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
            GPFCON &= ~(0x3<<(4*2));
            GPFCON |= (1<<(4*2));
            
            //s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
            GPFCON &= ~(0x3<<(5*2));
            GPFCON |= (1<<(5*2));

            //s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
            GPFCON &= ~(0x3<<(6*2));
            GPFCON |= (1<<(6*2));

            // �����0
            //s3c2410_gpio_setpin(S3C2410_GPF4, 0);
            GPFDAT &= ~(1<<4);
            
            //s3c2410_gpio_setpin(S3C2410_GPF5, 0);
            GPFDAT &= ~(1<<5);
            //s3c2410_gpio_setpin(S3C2410_GPF6, 0);
            GPFDAT &= ~(1<<6);

            down(&leds_lock);
            leds_status = 0x0;
            up(&leds_lock);
                
            break;
        }

        case 1: /* /dev/led1 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF4, 0);
            
            down(&leds_lock);
            leds_status &= ~(1<<0);
            up(&leds_lock);
            
            break;
        }

        case 2: /* /dev/led2 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF5, 0);
            leds_status &= ~(1<<1);
            break;
        }

        case 3: /* /dev/led3 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF6, 0);

            down(&leds_lock);
            leds_status &= ~(1<<2);
            up(&leds_lock);
            
            break;
        }
        
	}
	
    return 0;
}



static int s3c24xx_leds_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	int minor = MINOR(filp->f_dentry->d_inode->i_rdev);
    char val;

    switch (minor)
    {
        case 0: /* /dev/leds */
        {
            
            copy_to_user(buff, (const void *)&leds_status, 1);                    
            break;
        }

        case 1: /* /dev/led1 */
        {
            down(&leds_lock);
            val = leds_status & 0x1;
            up(&leds_lock);
            copy_to_user(buff, (const void *)&val, 1);
            break;
        }

        case 2: /* /dev/led2 */
        {
            down(&leds_lock);
            val = (leds_status>>1) & 0x1;
            up(&leds_lock);
            copy_to_user(buff, (const void *)&val, 1);
            break;
        }

        case 3: /* /dev/led3 */
        {
            down(&leds_lock);
            val = (leds_status>>2) & 0x1;
            up(&leds_lock);
            copy_to_user(buff, (const void *)&val, 1);
            break;
        }
        
    }

    return 1;
}




static ssize_t s3c24xx_leds_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
    //int minor = MINOR(inode->i_rdev); //MINOR(inode->i_cdev);
	int minor = MINOR(file->f_dentry->d_inode->i_rdev);
    char val;

    copy_from_user(&val, buf, 1);

    switch (minor)
    {
        case 0: /* /dev/leds */
        {            
            s3c2410_gpio_setpin(S3C2410_GPF4, (val & 0x1));
            s3c2410_gpio_setpin(S3C2410_GPF5, (val & 0x1));
            s3c2410_gpio_setpin(S3C2410_GPF6, (val & 0x1));
            down(&leds_lock);
            leds_status = val;
            up(&leds_lock);
            break;
        }

        case 1: /* /dev/led1 */
        {
            s3c2410_gpio_setpin(S3C2410_GPF4, val);

            if (val == 0)
            {
                down(&leds_lock);
                leds_status &= ~(1<<0);
                up(&leds_lock);
            }
            else
            {
                down(&leds_lock);
                leds_status |= (1<<0);                
                up(&leds_lock);
            }
            break;
        }

        case 2: /* /dev/led2 */
        {
            s3c2410_gpio_setpin(S3C2410_GPF5, val);
            if (val == 0)
            {
                down(&leds_lock);
                leds_status &= ~(1<<1);
                up(&leds_lock);
            }
            else
            {
                down(&leds_lock);
                leds_status |= (1<<1);                
                up(&leds_lock);
            }
            break;
        }

        case 3: /* /dev/led3 */
        {
            s3c2410_gpio_setpin(S3C2410_GPF6, val);
            if (val == 0)
            {
                down(&leds_lock);
                leds_status &= ~(1<<2);
                up(&leds_lock);
            }
            else
            {
                down(&leds_lock);
                leds_status |= (1<<2);                
                up(&leds_lock);
            }
            break;
        }
        
    }

    return 1;
}



/* ����ṹ���ַ��豸��������ĺ���
 * ��Ӧ�ó�������豸�ļ�ʱ�����õ�open��read��write�Ⱥ�����
 * ���ջ��������ṹ��ָ���Ķ�Ӧ����
 */
static struct file_operations s3c24xx_leds_fops = {
    .owner  =   THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
    .open   =   s3c24xx_leds_open,     
	.read	=	s3c24xx_leds_read,	   
	.write	=	s3c24xx_leds_write,	   
};

/*
 * ִ��insmod����ʱ�ͻ����������� 
 */
static int __init s3c24xx_leds_init(void)
//static int __init init_module(void)

{
    int ret;
	int minor = 0;

    gpio_va = ioremap(0x56000000, 0x100000);
	if (!gpio_va) {
		return -EIO;
	}

    /* ע���ַ��豸
     * ����Ϊ���豸�š��豸���֡�file_operations�ṹ��
     * ���������豸�žͺ;����file_operations�ṹ��ϵ�����ˣ�
     * �������豸ΪLED_MAJOR���豸�ļ�ʱ���ͻ����s3c24xx_leds_fops�е���س�Ա����
     * LED_MAJOR������Ϊ0����ʾ���ں��Զ��������豸��
     */
    ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &s3c24xx_leds_fops);
    if (ret < 0) {
      printk(DEVICE_NAME " can't register major number\n");
      return ret;
    }

	leds_class = class_create(THIS_MODULE, "leds");
	if (IS_ERR(leds_class))
		return PTR_ERR(leds_class);
    


	leds_class_devs[0] = class_device_create(leds_class, NULL, MKDEV(LED_MAJOR, 0), NULL, "leds"); /* /dev/leds */
	
	for (minor = 1; minor < 4; minor++)  /* /dev/led1,2,3 */
	{
		leds_class_devs[minor] = class_device_create(leds_class, NULL, MKDEV(LED_MAJOR, minor), NULL, "led%d", minor);
		if (unlikely(IS_ERR(leds_class_devs[minor])))
			return PTR_ERR(leds_class_devs[minor]);
	}
        
    printk(DEVICE_NAME " initialized\n");
    return 0;
}

/*
 * ִ��rmmod����ʱ�ͻ����������� 
 */
static void __exit s3c24xx_leds_exit(void)
{
	int minor;
    /* ж���������� */
    unregister_chrdev(LED_MAJOR, DEVICE_NAME);

	for (minor = 0; minor < 4; minor++)
	{
		class_device_unregister(leds_class_devs[minor]);
	}
	class_destroy(leds_class);
    iounmap(gpio_va);
}

/* ������ָ����������ĳ�ʼ��������ж�غ��� */
module_init(s3c24xx_leds_init);
module_exit(s3c24xx_leds_exit);

/* �������������һЩ��Ϣ�����Ǳ���� */
MODULE_AUTHOR("http://www.100ask.net");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("S3C2410/S3C2440 LED Driver");
MODULE_LICENSE("GPL");



