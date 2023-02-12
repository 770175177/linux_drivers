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
#include <asm/uaccess.h>

#define DRIVER_NAME "pollkey"
#define DEVICE_NAME "pollkey" 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static int pollkey_gpios[] = {
	EXYNOS4_GPX1(1),
	EXYNOS4_GPX1(2)
};

static int pollkey_open (struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "\tpollkey open\n");

	return 0;
}

static int pollkey_release (struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "\tpollkey release\n");

	return 0;
}

static size_t pollkey_read (struct file *file, char __user *buff, size_t size, loff_t *ppos)
{
	unsigned char key_value[2];

	if (size != sizeof(pollkey_gpios))
	{
		return -1;
	}

	key_value[0] = gpio_get_value(pollkey_gpios[0]);
	key_value[1] = gpio_get_value(pollkey_gpios[1]);

	copy_to_user(buff, key_value, sizeof(pollkey_gpios));

	return 0;
}

static const struct file_operations pollkey_fops = {			
	.owner	 = THIS_MODULE,	
	.open	 = pollkey_open,
	.release = pollkey_release,	
	.read	 = pollkey_read,	
};

static struct miscdevice pollkey_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &pollkey_fops
};

static int pollkey_probe(struct platform_device *pdev)
{
	int ret, i;
	
	printk("pollkey initialize\n");

	for (i = 0; i < 2; i++)
	{
		ret = gpio_request(pollkey_gpios[i], "gpio_input");
		s3c_gpio_cfgpin(pollkey_gpios[i], S3C_GPIO_INPUT);
		s3c_gpio_setpull(pollkey_gpios[i], S3C_GPIO_PULL_NONE);
	}

	ret = misc_register(&pollkey_dev);

	return 0;
}

static int pollkey_remove(struct platform_device *pdev)
{
	printk(KERN_EMERG "\tremoved\n");
	misc_deregister(&pollkey_dev);

	return 0;
}

static void pollkey_shutdown(struct platform_device *pdev)
{

	return ;
}

static int pollkey_suspend(struct platform_device *pdev, pm_message_t state)
{

	return 0;
}

static int pollkey_resume(struct platform_device *pdev)
{

	return 0;
}


struct platform_driver pollkey_driver = {
	.probe = pollkey_probe,
	.remove = pollkey_remove,
	.shutdown = pollkey_shutdown,
	.suspend = pollkey_suspend,
	.resume = pollkey_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static int pollkey_init(void)
{
	int driverState;

	driverState = platform_driver_register(&pollkey_driver);

	return driverState;
}

static void pollkey_exit(void)
{
	platform_driver_unregister(&pollkey_driver);

	return ;
}

module_init(pollkey_init);
module_exit(pollkey_exit);
