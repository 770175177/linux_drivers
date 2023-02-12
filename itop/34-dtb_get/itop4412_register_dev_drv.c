#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

#define DRIVER_NAME "leds_test"

static int leds_probe(struct platform_device * pdev)
{
	struct device_node *node = pdev->dev.of_node;

    printk("node name is %s\n", node->name);
    printk("node fullname is %s\n", node->full_name);

	struct property *comprop = NULL;
	comprop = of_find_property(node, "compatible", NULL);
	printk("comprop name is %s\n", comprop->name);
	printk("comprop value is %s\n", comprop->value);
	
	printk(KERN_ALERT "probe init\n");

	return 0;
}

static int leds_remove(struct platform_device * pdev)
{
	printk(KERN_ALERT "Goodbye, curel world, this is remove\n");
    return 0;
}

static const struct of_device_id of_leds_dt_match[] = {
	{.compatible = DRIVER_NAME},
	{},
};

MODULE_DEVICE_TABLE(of,of_leds_dt_match);

static struct platform_driver leds_driver = {
	.probe 	= leds_probe,
	.remove = leds_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_leds_dt_match,
		},
};

static int leds_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return platform_driver_register(&leds_driver);
	return 0;
}

static void leds_exit(void)
{
	printk(KERN_ALERT "Goodbye, curel world\n");
	platform_driver_unregister(&leds_driver);
}

module_init(leds_init);
module_exit(leds_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("rty");
MODULE_DESCRIPTION("topeet4412_regiter_dev_drv");
