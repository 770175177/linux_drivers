#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static int hello_init(void)
{
	printk(KERN_EMERG "Hello World enter!\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);
