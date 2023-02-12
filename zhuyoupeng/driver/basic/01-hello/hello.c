#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/moduleparam.h>

static char *who = "world";
static int howmany = 1;

module_param(howmany, int, S_IRUGO);
module_param(who, charp, S_IRUGO);

static int __init chrdev_init(void)
{
	printk(KERN_ALERT "chrdev_init, this is my first driver\n");
	while(howmany--)
	{
		printk(KERN_ALERT "hello %s\n", who);
	}
	printk(KERN_ALERT "The Process is \"%s\" (pid %i)\n", current->comm, current->pid);

	return 0;
}

static void __exit chrdev_exit(void)
{
	printk(KERN_ALERT "chrdev_exit, good bye\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Heng");
MODULE_DESCRIPTION("Module for test");
MODULE_ALIAS("heng:hi");
