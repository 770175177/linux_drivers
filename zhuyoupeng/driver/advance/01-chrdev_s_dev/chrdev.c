#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio-exynos4.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/cdev.h>

//#define USE_OLD_REGISTER_API

#define MYMAJOR 200
#define MYCNT   1
#define MYNAME  "testChar"

#define GPL2CON_PA 0x11000100
#define GPL2DAT_PA 0x11000104

//#define rGL2CON_PA *((volatile unsigned int *)GPK1CON)
//#define rGL2DAT_PA *((volatile unsigned int *)GPK1DAT)

#define GPK1CON_PA 0x11000060
#define GPK1DAT_PA 0x11000064

//#define rGPK1CON_PA *((volatile unsigned int *)GPK1CON)
//#define rGPK1DAT_PA *((volatile unsigned int *)GPK1DAT)

int mymajor;
char kbuf[100];

unsigned int *pGPL2CON;
unsigned int *pGPL2DAT;

static dev_t mydev;
static struct cdev test_cdev;

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
#if defined USE_OLD_REGISTER_API   
	// old register
	// 2. register a chrdev
	// major is 0, so kernel auto define a major number
	mymajor = register_chrdev(0, MYNAME, &test_fops);
	if(mymajor < 0)
	{
		printk(KERN_ERR "register chrdev fail\n");
		return -EINVAL;
	}
	printk(KERN_INFO "register chrdev success, my major is %d\n", mymajor);
sdsada
#endif
	// new register cdev has two step2
	// step 1: register master and slave device number
	mydev = MKDEV(MYMAJOR, 0);
	ret = register_chrdev_region(mydev, MYCNT, MYNAME);
	if(ret)
	{
		printk(KERN_ERR "Unable to register minors of %s", MYNAME);
		return -EINVAL;
	}
	printk(KERN_INFO "get device major 200 ok\n");
	// step 2: register a device
	cdev_init(&test_cdev, &test_fops);
	ret = cdev_add(&test_cdev, mydev, MYCNT);
	if (ret)
	{
		printk(KERN_ERR "Unable to cdev_add\n");
		return -EINVAL;
	}	
	printk(KERN_INFO "cdev_add success\n");

	// dynamic map physical addr to virtual addr
	if(!request_mem_region(GPL2CON_PA, 4, "GPL2CON"))
		return -EINVAL;
	if(!request_mem_region(GPL2DAT_PA, 4, "GPL2DAT"))
		return -EINVAL;
	pGPL2CON = ioremap(GPL2CON_PA, 4);
	pGPL2DAT = ioremap(GPL2DAT_PA, 4);

	// set regs
	*pGPL2CON = 0x11111111;
	*pGPL2DAT = 0x0;

	return 0;
}

static void __exit chrdev_exit(void)
{
	printk(KERN_INFO "chrdev_exit, good bye\n");

	*pGPL2DAT = 0x1;

	// unmap physical addr to virtual addr
	iounmap(pGPL2CON);
	iounmap(pGPL2DAT);
	release_mem_region(GPL2CON_PA, 4);
	release_mem_region(GPL2DAT_PA, 4);

	// 3. cancellation a chrdev
#if defined USE_OLD_REGISTER_API
	unregister_chrdev(mymajor, MYNAME);
#endif
	// use new API to unregister, has 2 steps
	// steps 1: unresiter chedev - cdev_del
	cdev_del(&test_cdev);

	// step 2: unregiter master and slave device number
	unregister_chrdev_region(mydev, MYCNT);
}


static int test_chrdev_open(struct inode *inode, struct file *file)
{
	//int ret = -1;

	// operate hardware, use printk instead
	printk(KERN_INFO "test chrdev open\n");
#if 0
	//  210	
	rGPK1CON = 0x11111111;
	rGPK1DAT = (0<<1);
	printk(KERN_INFO "GPK1CON = %p\n", GPK1CON);
	printk(KERN_INFO "GPK1DAT = %p\n", GPK1DAT);
#endif
#if 0
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

static int test_chrdev_relase(struct inode *inode, struct file *file)
{
	//int ret = -1;

	// release hardware, use printk instead
	printk(KERN_INFO "test chrdev close\n");
#if 0
	//  210
	rGPK1DAT = (1<<1);
#endif
#if 0
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
	memset(kbuf, 0, sizeof(kbuf));
	ret = copy_from_user(kbuf, ubuf, count);
	if(ret)
	{
		printk(KERN_ERR "copy from user fail\n");
		return -EINVAL;
	}
	printk(KERN_ERR "copy from user success\n");
	// refer data from user to write hardware
	if(!strncasecmp(kbuf, "on", 2))
	{
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
		printk("req ret = %d\n", ret);
		//if(ret < 0)
		//{
		//	printk(KERN_ERR "failed to request LED2\n");
		//	return ret;
		//}
		printk("LED3");
		gpio_direction_output(EXYNOS4_GPL2(0), 0);
		s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
		gpio_set_value(EXYNOS4_GPL2(0), 1);
		gpio_free(EXYNOS4_GPL2(0));
	}
	else if(!strncasecmp(kbuf, "off", 3))
	{
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
		gpio_set_value(EXYNOS4_GPK1(1), 0);
		gpio_free(EXYNOS4_GPK1(1));

		ret = gpio_request(EXYNOS4_GPL2(0), "LED3");
		printk("req ret = %d\n", ret);
		//if(ret < 0)
		//{
		//	printk(KERN_ERR "failed to request LED2\n");
		//	return ret;
		//}
		printk("LED3");
		gpio_direction_output(EXYNOS4_GPL2(0), 0);
		s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
		gpio_set_value(EXYNOS4_GPL2(0), 0);
		gpio_free(EXYNOS4_GPL2(0));
	}

	return 0;
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Heng");
MODULE_DESCRIPTION("Module for test");
MODULE_ALIAS("heng:hi");
