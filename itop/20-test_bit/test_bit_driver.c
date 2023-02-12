#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <asm/types.h>

#define DRIVER_NAME "atomic_bit"
#define DEVICE_NAME "atomic_bit"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

//static atomic_t value_atomic = ATOMIC_INIT(0);

unsigned long int value_bit = 0;

static int atomic_bit_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "atomic_bit_open in!\n");

	/*if (atomic_read(&value_atomic))
	{
		return -EBUSY;
	}

	atomic_inc(&value_atomic);
	*/

	if (test_bit(0, &value_bit) != 0)
	{
		return -EBUSY;
	}

	set_bit(0, &value_bit);

	printk(KERN_EMERG "atomic_bit_open success\n");

	return 0;
}

static int atomic_bit_release(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "atomic_bit_release\n");

	//atomic_dec(&value_atomic);

	clear_bit(0, &value_bit);

	return 0;
}

static struct file_operations atomic_bit_ops = {
	.owner = THIS_MODULE,
	.open = atomic_bit_open,
	.release = atomic_bit_release,
};

static struct miscdevice atomic_bit_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &atomic_bit_ops,
};

static int atomic_bit_probe (struct platform_device *pdev)
{
	printk(KERN_EMERG "\tinitialized\n");

	misc_register(&atomic_bit_dev);

	return 0;
}

static int atomic_bit_remove (struct platform_device *pdev)
{
	printk(KERN_EMERG "\tremove\n");

	misc_deregister(&atomic_bit_dev);
	
	return 0;
}

static struct platform_driver atomic_bit_driver = {
	.probe = atomic_bit_probe,
	.remove = atomic_bit_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init atomic_bit_init(void)
{
	int DriverState;

	printk(KERN_EMERG "Hello World enter!\n");

	DriverState = platform_driver_register(&atomic_bit_driver);

	printk(KERN_EMERG "\tDriverState is %d\n", DriverState);

	return 0;
}

static void atomic_bit_exit(void)
{
	//printk(KERN_EMERG "Hello World exit!\n");
	platform_driver_unregister(&atomic_bit_driver);
}

module_init(atomic_bit_init);
module_exit(atomic_bit_exit);
