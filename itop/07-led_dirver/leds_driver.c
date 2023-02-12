#include <linux/init.h>
#include <linux/module.h>
/* driver register related */
#include <linux/platform_device.h>
/* misc device register related */
#include <linux/miscdevice.h>
/* file device node struct related */
#include <linux/fs.h>

/* linux apply gpio */
#include <linux/gpio.h>
/* sumsang GPIO config */
/* sumsang exynos paltform gpio parameter */
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/* sumsang 4412 platform gpio macro */
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "hello_ctl"
#define DEVICE_NAME "hello_ctl123" 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static int hello_open (struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "\thello open\n");

	return 0;
}

static int hello_release (struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "\thello release\n");

	return 0;
}

static long hello_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	printk("cmd is %d, arg is %lu\n", cmd, arg);
	
	if (cmd > 1)
	{
		printk(KERN_EMERG "cmd is 0 or 1\n");
	}
	if (arg > 1)
	{
		printk(KERN_EMERG "arg is only 0 or 1\n");
	}

	gpio_set_value(EXYNOS4_GPL2(0), arg);

	return 0;
}

static const struct file_operations hello_fops = {			
	.owner	 = THIS_MODULE,	
	.open	 = hello_open,
	.release = hello_release,	
	.unlocked_ioctl	 = hello_ioctl,	
};

static struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &hello_fops
};

static int hello_probe(struct platform_device *pdev)
{
	int ret;

	printk(KERN_EMERG "\tinitialized\n");

	ret = gpio_request(EXYNOS4_GPL2(0), "LEDS");
	if (ret < 0)
	{
		printk(KERN_EMERG "gpio request EXYNOS4_GPL2(0) failed!\n");
		return ret;
	}
	gpio_set_value(EXYNOS4_GPL2(0), 0);

	s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);

	misc_register(&hello_dev);

	return 0;
}

static int hello_remove(struct platform_device *pdev)
{
	printk(KERN_EMERG "\tremoved\n");
	misc_deregister(&hello_dev);

	return 0;
}

static void hello_shutdown(struct platform_device *pdev)
{

	return ;
}

static int hello_suspend(struct platform_device *pdev, pm_message_t state)
{

	return 0;
}

static int hello_resume(struct platform_device *pdev)
{

	return 0;
}


struct platform_driver hello_driver = {
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static int hello_init(void)
{
	int driverState;

	printk(KERN_EMERG "Hello World enter!\n");
	driverState = platform_driver_register(&hello_driver);
	printk(KERN_EMERG "\tdriverState is %d\n", driverState);
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	platform_driver_unregister(&hello_driver);

	return ;
}

module_init(hello_init);
module_exit(hello_exit);
