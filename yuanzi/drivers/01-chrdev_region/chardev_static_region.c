#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#define CHAR_MAJOR    200
#define CHAR_NAME     "chardev"

struct class *myclass = NULL;
struct device *mydevice = NULL;
dev_t mydev = 0;

static char readbuf[100];
static char writebuf[100];
static char kerneldata[] = {"kernel data!"};

static int chardev_open(struct inode *inode, struct file *filp)
{
	printk("chardev open\n");
	return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int ret = 0;

	memset(readbuf, 0, sizeof(readbuf));
	memcpy(readbuf, kerneldata, sizeof(kerneldata));
	ret = copy_to_user(buf, readbuf, cnt);
	if(ret < 0)
	{
		printk("kernel send data failed\n");
	}
	else
	{
		printk("kernel send data success\n");
	}

	return 0;
}

static ssize_t chardev_write(struct file *filep, const char __user *buf, size_t cnt, loff_t *offt)
{
	int ret = 0;

	memset(writebuf, 0, sizeof(writebuf));
	ret = copy_from_user(writebuf, buf, cnt);
	if(ret < 0)
	{
		printk("kernel receive failed\n");
	}
	else
	{
		printk("kernel receive: %s\n", writebuf);
	}

	return 0;
}

static int chardev_release(struct inode *inode, struct file *filp)
{
	printk("chardev release\n");
	
	return 0;
}

static struct file_operations chardev_fops = {
	.owner   = THIS_MODULE,
	.open    = chardev_open,
	.read    = chardev_read,
	.write   = chardev_write,
	.release = chardev_release,
};

static int __init chardev_init(void)
{
	int ret = 0;

	ret = register_chrdev(CHAR_MAJOR, CHAR_NAME, &chardev_fops);
	if (ret < 0)
	{
		printk("chardev driver register failed, ret = %d\n", ret);
		return -1;
	}
	printk("chardev init success\n");

	mydev = MKDEV(CHAR_MAJOR, 0);
	myclass = class_create(THIS_MODULE, CHAR_NAME);
	if (IS_ERR(myclass))
	{
		printk("create class failed\n");
		goto err1;
	}
	printk("create class success\n");
	
	mydevice = device_create(myclass, NULL, mydev, NULL, CHAR_NAME);
	if (IS_ERR(myclass))
	{
		printk("create device failed\n");
		goto err2;
	}
	printk("create device success\n");
	goto success;

err2:
	class_destroy(myclass);
err1:
	unregister_chrdev(CHAR_MAJOR, CHAR_NAME);
	return -1;
success:
	return 0;
}

static void __exit chardev_exit(void)
{
	device_destroy(myclass, mydev);
	class_destroy(myclass);
	unregister_chrdev(CHAR_MAJOR, CHAR_NAME);
	printk("chardev exit\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heng");
