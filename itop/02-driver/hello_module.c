#include <linux/init.h>
#include <linux/module.h>
/* driver register related */
#include <linux/platform_device.h>

#define DRIVER_NAME "hello_ctl"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static int hello_probe(struct platform_device *pdev)
{
	printk(KERN_EMERG "\tinitialized\n");

	return 0;
}

static int hello_remove(struct platform_device *pdev)
{
	printk(KERN_EMERG "\tremoved\n");

	return 0;
}

static void hello_shutdown(struct platform_device *pdev)
{

	return ;
}

static int hello_suspend(struct platform_device *pdev, pm_message_t state)
{

	return 0;
}

static int hello_resume(struct platform_device *pdev)
{

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

static int hello_init(void)
{
	int driverState;

	printk(KERN_EMERG "Hello World enter!\n");
	driverState = platform_driver_register(&hello_driver);
	printk(KERN_EMERG "\tdriverState is %d\n", driverState);
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	platform_driver_unregister(&hello_driver);

	return ;
}

module_init(hello_init);
module_exit(hello_exit);
