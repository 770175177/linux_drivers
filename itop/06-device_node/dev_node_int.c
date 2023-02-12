#include <linux/init.h>
#include <linux/module.h>
/* driver register related */
#include <linux/platform_device.h>
/* misc device register related */
#include <linux/miscdevice.h>
/* file device node struct related */
#include <linux/fs.h>

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

static int hello_init(void)
{
	int driverState;

	printk(KERN_EMERG "Hello World enter!\n");
	misc_register(&hello_dev);
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	misc_deregister(&hello_dev);

	return ;
}

module_init(hello_init);
module_exit(hello_exit);
