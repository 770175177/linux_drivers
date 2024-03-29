#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio-exynos4.h>
#include <linux/gpio.h>
#include <mach/gpio.h>

//#define MYMAJOR 200
#define MYNAME  "test char device"

#define GPK1CON 0x11000060
#define GPK1DAT 0x11000064

#define rGPK1CON *((volatile unsigned int *)GPK1CON)
#define rGPK1DAT *((volatile unsigned int *)GPK1DAT)

int mymajor;
char kbuf[100];

static int test_chrdev_open (struct inode *, struct file *);
static int test_chrdev_relase (struct inode *, struct file *);
static ssize_t test_chrdev_read (struct file *, char __user *, size_t, loff_t *);
static ssize_t test_chrdev_write (struct file *, const char __user *, size_t, loff_t *);

// 1. define a file_operations struct, and fill it
const struct file_operations test_fops = {
	.owner          = THIS_MODULE,
	.open		= test_chrdev_open,   // for app open
	.release	= test_chrdev_relase,  // for app close
	.write          = test_chrdev_write,
	.read           = test_chrdev_read,
};

static int __init chrdev_init(void)
{
	int ret = -1;

	printk(KERN_INFO "chrdev_init, this is my first driver\n");
	// 2. register a chrdev
	// major is 0, so kernel auto define a major number
	mymajor = register_chrdev(0, MYNAME, &test_fops);
	if(mymajor < 0)
	{
		printk(KERN_ERR "register chrdev fail\n");
		return -EINVAL;
	}
	printk(KERN_INFO "register chrdev success, my major is %d\n", mymajor);

#if 0
	//  210	
	rGPK1CON = 0x11111111;
	rGPK1DAT = (0<<1);
	printk(KERN_INFO "GPK1CON = %p\n", GPK1CON);
	printk(KERN_INFO "GPK1DAT = %p\n", GPK1DAT);
#endif
#if 1
	//  4412
	ret = gpio_request(EXYNOS4_GPK1(1), "LED2");
	printk("req ret = %d\n", ret);
	//if(ret < 0)
	//{
	//	printk(KERN_ERR "failed to request LED2\n");
	//	return ret;
	//}
	printk("LED2");
	gpio_direction_output(EXYNOS4_GPK1(1), 0);
	s3c_gpio_cfgpin(EXYNOS4_GPK1(1), S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPK1(1), 1);
	gpio_free(EXYNOS4_GPK1(1));

	ret = gpio_request(EXYNOS4_GPL2(0), "LED3");
	//if (ret < 0)
	//{
//		printk(KERN_ERR "failed to request LED3\n");
//		return ret;
//	}
	printk("LED3");
	s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPL2(0), 0);
	gpio_free(EXYNOS4_GPL2(0));
#endif

	return 0;
}

static void __exit chrdev_exit(void)
{
	int ret = -1;

	printk(KERN_INFO "chrdev_exit, good bye\n");
#if 0
	//  210
	rGPK1DAT = (1<<1);
#endif
#if 1
	//  4412
	ret = gpio_request(EXYNOS4_GPK1(1), "LED2");
//	if (ret < 0)
//	{
//		printk(KERN_ERR "failed to request LED2\n");
//		return ;
//	}
	gpio_direction_output(EXYNOS4_GPK1(1), 1);
	s3c_gpio_cfgpin(EXYNOS4_GPK1(1), S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPK1(1), 0);
	gpio_free(EXYNOS4_GPK1(1));

	ret = gpio_request(EXYNOS4_GPL2(0), "LED3");
//	if (ret < 0)
//	{
//		printk(KERN_ERR "failed to request LED3\n");
//		return ;
//	}
	printk("LED3");
	s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPL2(0), 1);
	gpio_free(EXYNOS4_GPL2(0));
#endif
	// 3. cancellation a chrdev
	unregister_chrdev(mymajor, MYNAME);
}


static int test_chrdev_open(struct inode *inode, struct file *file)
{
	// operate hardware, use printk instead
	printk(KERN_INFO "test chrdev open\n");

	return 0;
}

static int test_chrdev_relase(struct inode *inode, struct file *file)
{
	// release hardware, use printk instead
	printk(KERN_INFO "test chrdev close\n");

	return 0;
}

static ssize_t test_chrdev_read (struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	int ret = -1;

	// operate hardware, use printk instead
	printk(KERN_INFO "test chrdev read\n");
	// copy kernel buf to user
	ret = copy_to_user(ubuf, kbuf, count);
	if(ret)
	{
		printk(KERN_ERR "copy to user fail\n");
		return -EINVAL;
	}
	printk(KERN_ERR "copy to user success\n");

	return 0;
}

static ssize_t test_chrdev_write (struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
	int ret = -1;

	// operate hardware, use printk instead
	printk(KERN_INFO "test chrdev write\n");
	// copy from user to a kernel buf
	ret = copy_from_user(kbuf, ubuf, count);
	if(ret)
	{
		printk(KERN_ERR "copy from user fail\n");
		return -EINVAL;
	}
	printk(KERN_ERR "copy from user success\n");
	// refer data from user to write hardware

	
	return 0;
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Heng");
MODULE_DESCRIPTION("Module for test");
MODULE_ALIAS("heng:hi");
