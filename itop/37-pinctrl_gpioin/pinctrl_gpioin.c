#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define DRIVER_NAME "gpio_in"

int gpio_pin = 0;

int gpio_open(struct inode *inode, struct file *filp)
{
	printk("Device Opened Success!\n");

	return nonseekable_open(inode, filp);
}

int gpio_release(struct inode *inode, struct file *filp)
{
	printk("Device Closed Success!\n");

	return 0;
}

long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("debug: gpio_ioctl cmd is %d, arg is %ld\n", cmd, arg);

	switch(cmd)
	{
		case 0:
		case 1:
			return gpio_get_value(gpio_pin);
		default:
			return -EINVAL;
	}

	return 0;
}

static struct file_operations gpio_ops = {
	.owner = THIS_MODULE,
	.open  = gpio_open,
	.release = gpio_release,
	.unlocked_ioctl = gpio_ioctl,
};

static struct miscdevice gpio_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops  = &gpio_ops,
	.name  = DRIVER_NAME,
};

static int gpio_probe(struct platform_device * pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int ret, value;

	printk(KERN_ALERT "gpio_probe init\n");

	gpio_pin = of_get_named_gpio(node, "gpioin", 0);
	if (gpio_pin < 0)
	{
		printk("gpio pin is not avalible\n");
		return -1;
	}

	ret = gpio_request(gpio_pin, "gpio-in");
	if (ret < 0)
	{
		printk("gpio_pin_request %d failed\n", gpio_pin);
		return ret;
	}
	printk("gpio_pin is %d\n", gpio_pin);

	gpio_free(gpio_pin);
	gpio_direction_input(gpio_pin);

	value = gpio_get_value(gpio_pin);
	printk("the pin value is %d\n", value);
	
	ret = misc_register(&gpio_dev);
	if (ret < 0)
	{
		printk("gpio_register device failed\n");
		goto exit;
	}

	return 0;
exit:
	misc_deregister(&gpio_dev);
	return ret;
}

static int gpio_remove(struct platform_device * pdev)
{
	printk(KERN_ALERT "Goodbye, curel world, this is remove\n");
	misc_deregister(&gpio_dev);

    return 0;
}

static const struct of_device_id of_gpio_dt_match[] = {
	{.compatible = DRIVER_NAME},
	{},
};

MODULE_DEVICE_TABLE(of,of_gpio_dt_match);

static struct platform_driver gpio_driver = {
	.probe 	= gpio_probe,
	.remove = gpio_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_gpio_dt_match,
		},
};

static int gpio_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return platform_driver_register(&gpio_driver);
	return 0;
}

static void gpio_exit(void)
{
	printk(KERN_ALERT "Goodbye, curel world\n");
	platform_driver_unregister(&gpio_driver);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("rty");
MODULE_DESCRIPTION("topeet4412_regiter_dev_drv");
