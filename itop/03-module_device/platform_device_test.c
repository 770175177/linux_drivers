#include <linux/init.h>
#include <linux/module.h>
/* driver register related */
#include <linux/platform_device.h>

#define DRIVER_NAME "my_code_led"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static void leds_release(struct device *pdev)
{
	printk("leds_release");
}

struct platform_device platform_device_hello = {
	.name = DRIVER_NAME,
	.id = -1,
	.dev = {
		.release = leds_release,
	}
};

static int hello_init(void)
{
	platform_device_register(&platform_device_hello);
	
	return 0;
}

static void hello_exit(void)
{
	platform_device_unregister(&platform_device_hello);

	return ;
}

module_init(hello_init);
module_exit(hello_exit);
