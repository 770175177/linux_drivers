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

static struct class *seconddrv_class;
static struct class_device	*seconddrv_class_dev;

volatile unsigned long *gpfcon;
volatile unsigned long *gpfdat;

volatile unsigned long *gpgcon;
volatile unsigned long *gpgdat;

static int second_drv_open(struct inode *inode, struct file *file)
{
	/* ����GPF0,2Ϊ�������� */
	*gpfcon &= ~((0x3<<(0*2)) | (0x3<<(2*2)));

	/* ����GPG3,11Ϊ�������� */
	*gpgcon &= ~((0x3<<(3*2)) | (0x3<<(11*2)));

	return 0;
}

ssize_t second_drv_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	/* ����4�����ŵĵ�ƽ */
	unsigned char key_vals[4];
	int regval;

	if (size != sizeof(key_vals))
		return -EINVAL;

	/* ��GPF0,2 */
	regval = *gpfdat;
	key_vals[0] = (regval & (1<<0)) ? 1 : 0;
	key_vals[1] = (regval & (1<<2)) ? 1 : 0;
	

	/* ��GPG3,11 */
	regval = *gpgdat;
	key_vals[2] = (regval & (1<<3)) ? 1 : 0;
	key_vals[3] = (regval & (1<<11)) ? 1 : 0;

	copy_to_user(buf, key_vals, sizeof(key_vals));
	
	return sizeof(key_vals);
}


static struct file_operations sencod_drv_fops = {
    .owner  =   THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
    .open   =   second_drv_open,     
	.read	=	second_drv_read,	   
};


int major;
static int second_drv_init(void)
{
	major = register_chrdev(0, "second_drv", &sencod_drv_fops);

	seconddrv_class = class_create(THIS_MODULE, "second_drv");

	seconddrv_class_dev = class_device_create(seconddrv_class, NULL, MKDEV(major, 0), NULL, "buttons"); /* /dev/buttons */

	gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
	gpfdat = gpfcon + 1;

	gpgcon = (volatile unsigned long *)ioremap(0x56000060, 16);
	gpgdat = gpgcon + 1;

	return 0;
}

static void second_drv_exit(void)
{
	unregister_chrdev(major, "second_drv");
	class_device_unregister(seconddrv_class_dev);
	class_destroy(seconddrv_class);
	iounmap(gpfcon);
	iounmap(gpgcon);
	return 0;
}


module_init(second_drv_init);

module_exit(second_drv_exit);

MODULE_LICENSE("GPL");

