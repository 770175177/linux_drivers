#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define MYMAJOR 200
#define MYNAME  "test char device"

static int test_chrdev_open (struct inode *, struct file *);
static int test_chrdev_relase (struct inode *, struct file *);

// 1. define a file_operations struct, and fill it
const struct file_operations test_fops = {
	.owner          = THIS_MODULE,
	.open		= test_chrdev_open,   // for app open
	.release	= test_chrdev_relase  // for app close
};

static int __init chrdev_init(void)
{
	int ret = -1;

	printk(KERN_INFO "chrdev_init, this is my first driver\n");
	// 2. register a chrdev
	ret = register_chrdev(MYMAJOR, MYNAME, &test_fops);
	if(ret)
	{
		printk(KERN_ERR "register chrdev fail\n");
		return -EINVAL;
	}
	printk(KERN_INFO "register chrdev success\n");

	return 0;
}

static void __exit chrdev_exit(void)
{
	printk(KERN_INFO "chrdev_exit, good bye\n");
	// 3. cancellation a chrdev
	unregister_chrdev(MYMAJOR, MYNAME);
}


static int test_chrdev_open(struct inode *inode, struct file *file)
{
	// operate hardware, use printk instead
	printk(KERN_INFO "test chrdev open\n");

	return 0;
}

static int test_chrdev_relase(struct inode *inode, struct file *file)
{
	// release hardware, use printk instead
	printk(KERN_INFO "test chrdev close");

	return 0;
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Heng");
MODULE_DESCRIPTION("Module for test");
MODULE_ALIAS("heng:hi");
