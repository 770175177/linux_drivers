#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/poll.h>
#include <linux/cdev.h>

/* 1. ȷ�����豸�� */
static int major;

static int hello_open(struct inode *inode, struct file *file)
{
	printk("hello_open\n");
	return 0;
}


/* 2. ����file_operations */
static struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open  = hello_open,
};

#define HELLO_CNT   2

static struct cdev hello_cdev;
static struct class *cls;

static int hello_init(void)
{
	dev_t devid;
	
	/* 3. �����ں� */
#if 0
	major = register_chrdev(0, "hello", &hello_fops); /* (major,  0), (major, 1), ..., (major, 255)����Ӧhello_fops */
#else
	if (major) {
		devid = MKDEV(major, 0);
		register_chrdev_region(devid, HELLO_CNT, "hello");  /* (major,0~1) ��Ӧ hello_fops, (major, 2~255)������Ӧhello_fops */
	} else {
		alloc_chrdev_region(&devid, 0, HELLO_CNT, "hello"); /* (major,0~1) ��Ӧ hello_fops, (major, 2~255)������Ӧhello_fops */
		major = MAJOR(devid);                     
	}
	
	cdev_init(&hello_cdev, &hello_fops);
	cdev_add(&hello_cdev, devid, HELLO_CNT);
#endif

	cls = class_create(THIS_MODULE, "hello");
	class_device_create(cls, NULL, MKDEV(major, 0), NULL, "hello0"); /* /dev/hello0 */
	class_device_create(cls, NULL, MKDEV(major, 1), NULL, "hello1"); /* /dev/hello1 */
	class_device_create(cls, NULL, MKDEV(major, 2), NULL, "hello2"); /* /dev/hello2 */
	
	
	return 0;
}

static void hello_exit(void)
{
	class_device_destroy(cls, MKDEV(major, 0));
	class_device_destroy(cls, MKDEV(major, 1));
	class_device_destroy(cls, MKDEV(major, 2));
	class_destroy(cls);

	cdev_del(&hello_cdev);
	unregister_chrdev_region(MKDEV(major, 0), HELLO_CNT);
}

module_init(hello_init);
module_exit(hello_exit);


MODULE_LICENSE("GPL");

