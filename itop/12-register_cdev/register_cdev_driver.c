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
/*allocate memery*/
#include <linux/slab.h>
/*device_create and class*/
#include <linux/device.h>

#define DEVICE_NAME  "dcdev"
#define DEVICE_MINOR_NUM 2
#define DEVICE_MAJOR     0
#define DEVICE_MINOR     0
#define REGDEV_SIZE      3000

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

int number_major = DEVICE_MAJOR;
int number_minor = DEVICE_MINOR;

/*input major device number*/
module_param(number_major, int, S_IRUSR);
/*input minor device number*/
module_param(number_minor, int, S_IRUSR);

static struct class *myclass;

struct reg_deg {
	char *data;
	unsigned long size;

	struct cdev cdev;
};

struct reg_deg *my_devices;

struct file_operations my_fops = {
	.owner  = THIS_MODULE,
};

static void reg_init_cdev(struct reg_deg *dev, int index)
{
	int err;
	int devno = MKDEV(number_major, number_minor + index);

	cdev_init(&dev->cdev, &my_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &my_fops;

	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
	{
		printk(KERN_EMERG "cdev_add %d is failed! ret %d\n", index, err);
	}
	else
	{
		printk(KERN_EMERG "cdev_add %d is success!\n", index);
	}
}

static int cDev_init(void)
{
	int ret = 0, i;
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
		/*register device number dynamicly*/
		ret = alloc_chrdev_region(&num_dev, number_minor, DEVICE_MINOR_NUM, DEVICE_NAME);
		/*get major device number*/
		number_major = MAJOR(num_dev);
		number_minor = MINOR(num_dev);
		printk(KERN_EMERG "alloc_chrdev_region major %d!\n", number_major);
	}

	if(ret < 0)
	{
		printk(KERN_EMERG "register_chardev_region request %d is failed!\n", number_major);
	}
	myclass = class_create(THIS_MODULE, DEVICE_NAME);

	my_devices = kmalloc(DEVICE_MINOR_NUM * sizeof(struct reg_deg), GFP_KERNEL);
	if (NULL == my_devices)
	{
		ret = -ENOMEM;
		goto fail;
	}
	memset(my_devices, 0, sizeof(DEVICE_MINOR_NUM * sizeof(struct reg_deg)));
	/*device initialize*/
	for (i = 0; i < DEVICE_MINOR_NUM; i++)
	{
		my_devices[i].data = kmalloc(REGDEV_SIZE, GFP_KERNEL);
		memset(&(my_devices[i].data), 0, REGDEV_SIZE);
		/*device register to system*/
		reg_init_cdev(&my_devices[i], i);
	}

	printk(KERN_EMERG "Cdev Enter!\n");
	
	return 0;

fail:
	/*unregister device number*/
	unregister_chrdev_region(MKDEV(number_major, number_minor), DEVICE_MINOR_NUM);
	printk(KERN_EMERG "kmalloc is failed!\n");
	return ret;
}

static void cDev_exit(void)
{
	int i;

	printk(KERN_EMERG "Cdev exit!\n");

	/*unregister char device*/
	for (i = 0; i < DEVICE_MINOR_NUM; i++)
	{
		cdev_del(&(my_devices[i].cdev));
		device_destroy(myclass, MKDEV(number_major, number_minor+i));
	}
	class_destroy(myclass);
	kfree(my_devices);

	unregister_chrdev_region(MKDEV(number_major, number_minor), DEVICE_MINOR_NUM);

	return ;
}

module_init(cDev_init);
module_exit(cDev_exit);
