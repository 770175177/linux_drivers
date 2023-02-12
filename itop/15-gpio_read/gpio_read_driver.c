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

#define DRIVER_NAME "gpio_read_ctl"
#define DEVICE_NAME "gpio_read_ctl" 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static int gpio_read_open (struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "\tgpio_read open\n");

	return 0;
}

static int gpio_read_release (struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "\tgpio_read release\n");

	return 0;
}

static long gpio_read_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
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

	// if cmd is 0, indicate switch 3
	// if cmd is 1, indicate switch 4
	if (cmd == 0)
	{
		return gpio_get_value(EXYNOS4_GPC0(3));
	}
	if (cmd == 1)
	{
		return gpio_get_value(EXYNOS4_GPX0(6));
	}

	return 0;
}

static const struct file_operations gpio_read_fops = {			
	.owner	 = THIS_MODULE,	
	.open	 = gpio_read_open,
	.release = gpio_read_release,	
	.unlocked_ioctl	 = gpio_read_ioctl,	
};

static struct miscdevice gpio_read_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &gpio_read_fops
};

static int gpio_read_probe(struct platform_device *pdev)
{
	int ret;

	printk(KERN_EMERG "\tinitialized\n");

	ret = gpio_request(EXYNOS4_GPC0(3), "GPIO_SWITCH_3");
	if (ret < 0)
	{
		printk(KERN_EMERG "gpio request EXYNOS4_GPC0(3) failed!\n");
		return ret;
	}
	else
	{
		s3c_gpio_cfgpin(EXYNOS4_GPC0(3), S3C_GPIO_INPUT);
		s3c_gpio_setpull(EXYNOS4_GPC0(3), S3C_GPIO_PULL_NONE);
	}

	ret = gpio_request(EXYNOS4_GPX0(6), "GPIO_SWITCH_4");
	if (ret < 0)
	{
		printk(KERN_EMERG "gpio request EXYNOS4_GPX0(6) failed!\n");
		return ret;
	}
	else
	{
		s3c_gpio_cfgpin(EXYNOS4_GPX0(6), S3C_GPIO_INPUT);
		s3c_gpio_setpull(EXYNOS4_GPX0(6), S3C_GPIO_PULL_NONE);
	}

	misc_register(&gpio_read_dev);

	return 0;
}

static int gpio_read_remove(struct platform_device *pdev)
{
	printk(KERN_EMERG "\tremoved\n");
	misc_deregister(&gpio_read_dev);

	return 0;
}

static void gpio_read_shutdown(struct platform_device *pdev)
{

	return ;
}

static int gpio_read_suspend(struct platform_device *pdev, pm_message_t state)
{

	return 0;
}

static int gpio_read_resume(struct platform_device *pdev)
{

	return 0;
}


struct platform_driver gpio_read_driver = {
	.probe = gpio_read_probe,
	.remove = gpio_read_remove,
	.shutdown = gpio_read_shutdown,
	.suspend = gpio_read_suspend,
	.resume = gpio_read_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static int gpio_read_init(void)
{
	int driverState;

	printk(KERN_EMERG "Hello World enter!\n");
	driverState = platform_driver_register(&gpio_read_driver);
	printk(KERN_EMERG "\tdriverState is %d\n", driverState);
	
	return 0;
}

static void gpio_read_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	platform_driver_unregister(&gpio_read_driver);

	return ;
}

module_init(gpio_read_init);
module_exit(gpio_read_exit);
