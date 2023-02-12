#include <linux/init.h>
#include <linux/module.h>
/*define module_param module_param_array*/
#include <linux/moduleparam.h>
/*define parameter of module_param and module_param_array*/
#include <linux/stat.h>
/*char device functions*/
#include <linux/fs.h>
/*MKDEV transfer device number*/
#include <linux/kdev_t.h>
/*define char device structure*/
#include <linux/cdev.h>

#define DEVICE_NAME  "dcdev"
#define DEVICE_MINOR_NUM 2
#define DEVICE_MAJOR     0
#define DEVICE_MINOR     0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

/*input major device number*/
int number_major = DEVICE_MAJOR;
/*input minor device number*/
int number_minor = DEVICE_MINOR;

module_param(number_major, int, S_IRUSR);

module_param(number_minor, int, S_IRUSR);

static int cDev_init(void)
{
	int ret = 0;
	dev_t num_dev;

	printk(KERN_EMERG "number_major is %d!\n", number_major);
	printk(KERN_EMERG "number_minor is %d!\n", number_minor);

	if(number_major)
	{
		num_dev = MKDEV(number_major, number_minor);
		ret = register_chrdev_region(num_dev, DEVICE_MINOR_NUM, DEVICE_NAME);
	}
	else
	{
		ret = alloc_chrdev_region(&num_dev, number_minor, DEVICE_MINOR_NUM, DEVICE_NAME);
		number_major = MAJOR(num_dev);
		number_minor = MINOR(num_dev);
		printk(KERN_EMERG "alloc_chrdev_region major %d!\n", number_major);
	}

	if(ret < 0)
	{
		printk(KERN_EMERG "register_chardev_region request %d is failed!\n", number_major);
	}

	printk(KERN_EMERG "Cdev Enter!\n");
	
	return 0;
}

static void cDev_exit(void)
{
	printk(KERN_EMERG "Cdev exit!\n");

	unregister_chrdev_region(MKDEV(number_major, number_minor), DEVICE_MINOR_NUM);

	return ;
}

module_init(cDev_init);
module_exit(cDev_exit);
