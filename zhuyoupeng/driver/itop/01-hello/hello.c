#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

static char *who = "world";
static int howmany = 1;
#define DRIVER_NAME "hello_ctl"
#define DEVICE_NAME "hello_ctl123"

module_param(howmany, int, S_IRUGO);
module_param(who, charp, S_IRUGO);

static int hello_open(struct inode *inode, struct file *filp)
{
	printk("hello_open\n");

	return 0;
}

static int hello_release(struct inode *inode, struct file *filp)
{
	printk("hello_release\n");

	return 0;
}

static long hello_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("hello_ioctl\n");
	printk("command is %d, arg is %ld\n", cmd, arg);

	return 0;
}

static struct file_operations hello_ops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_ioctl,
};

static struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &hello_ops,
};

static int hello_probe(struct platform_device *pdev)
{
	printk(KERN_ALERT "hello_probe\n");
	printk(KERN_ALERT "call misc_register to generate device node\n");
	misc_register(&hello_dev);

	return 0;
}

static int hello_remove(struct platform_device *pdev)
{
	printk(KERN_ALERT "hello_remove\n");
	printk(KERN_ALERT "call misc_deregister to generate device node\n");
	misc_deregister(&hello_dev);

	return 0;
}

static void hello_shutdown(struct platform_device *pdev)
{
	printk(KERN_ALERT "hello_shutdown\n");

	return;
}

static int hello_suspend(struct platform_device *pdev, pm_message_t state)
{
	printk(KERN_ALERT "hello_suspend\n");

	return 0;
}

static int hello_resume(struct platform_device *pdev)
{
	printk(KERN_ALERT "hello_resume\n");

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

static int __init hello_init(void)
{
	int ret;

	printk(KERN_ALERT "hello init, this is my first driver\n");
	while(howmany--)
	{
		printk(KERN_ALERT "hello %s\n", who);
	}
	printk(KERN_ALERT "The Process is \"%s\" (pid %i)\n", current->comm, current->pid);
	
	ret = platform_driver_register(&hello_driver);
	printk(KERN_ALERT "platform_driver_register ret %d\n", ret);	

	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "hello exit, good bye\n");

	platform_driver_unregister(&hello_driver);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Heng");
MODULE_DESCRIPTION("Module for test");
MODULE_ALIAS("heng:hi");
