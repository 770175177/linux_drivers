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

static int led_gpios[] = {
	EXYNOS4_GPL2(0), EXYNOS4_GPK1(1),
	EXYNOS4_GPD0(0),

	EXYNOS4_GPX1(0), EXYNOS4_GPX1(3), EXYNOS4_GPX1(5), EXYNOS4_GPX1(6),
	EXYNOS4_GPX3(0), EXYNOS4_GPX2(6), EXYNOS4_GPX2(7), EXYNOS4_GPX3(5),

	EXYNOS4212_GPJ1(3), EXYNOS4_GPL0(1), EXYNOS4_GPL0(3), EXYNOS4212_GPJ1(0),
	EXYNOS4212_GPJ1(2), EXYNOS4212_GPJ1(1), EXYNOS4212_GPJ0(7), EXYNOS4212_GPJ0(6),
	EXYNOS4212_GPJ0(5), EXYNOS4212_GPJ0(4), EXYNOS4212_GPJ0(0), EXYNOS4212_GPJ0(3),
	EXYNOS4212_GPJ0(1), EXYNOS4212_GPJ0(2),

	EXYNOS4_GPK3(6), EXYNOS4_GPK3(1), EXYNOS4_GPK3(4), EXYNOS4_GPK3(0),
	EXYNOS4_GPK3(3), EXYNOS4_GPK3(5), EXYNOS4_GPC1(1)
};

#define GPIOS_NUM (sizeof(led_gpios) / sizeof(int))

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

	switch(cmd)
	{
		case 0:
		case 1:
			if(arg > GPIOS_NUM)
			{
				return -EINVAL;
			}
			gpio_set_value(led_gpios[arg], cmd);
			break;
		default:
			return -EINVAL;
	}

	gpio_set_value(led_gpios[2], 0);

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
	int ret, i;

	printk(KERN_EMERG "\tinitialized\n");

	for (i = 0; i < GPIOS_NUM; i++)
	{
		ret = gpio_request(led_gpios[i], "GPIOS");
		if (ret < 0)
		{
			printk(KERN_EMERG "gpio request %d failed!\n", i);
			return ret;
		}
		else
		{
			s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
			gpio_set_value(led_gpios[i], 1);
		}
	}

	gpio_set_value(led_gpios[2], 0);

	ret = misc_register(&hello_dev);
	if(ret < 0)
	{
		printk("leds: register device failed!\n");
		goto exit;
	}

	return 0;

exit:
	misc_deregister(&hello_dev);
	return ret;
}

static int hello_remove(struct platform_device *pdev)
{
	int i;

	printk(KERN_EMERG "\tremoved\n");

	for(i = 0; i < GPIOS_NUM; i++)
	{
		gpio_free(led_gpios[i]);
	}	
	
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
