#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

#include <linux/miscdevice.h>
#include <linux/fs.h>

#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio_keys.h>
#include <linux/of_irq.h>

#define DRIVER_NAME "itop-4412,gpio-keys"

static irqreturn_t eint_interrupt(int irq, void *dev_id)
{
	printk("%s(%d)\n", __FUNCTION__, __LINE__);
	printk("HOME KEY HIGH TO LOW!\n");

	return IRQ_HANDLED;
}

static int inter_probe(struct platform_device * pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int ret, irq[2];

	irq[0] = irq_of_parse_and_map(node, 0);
	printk(KERN_ALERT "irq number is %d\n", irq[0]);

	irq[1] = irq_of_parse_and_map(node, 1);
	printk(KERN_ALERT "irq number is %d\n", irq[1]);

	printk(KERN_ALERT "inter_probe init ok\n");

	return 0;
}

static int inter_remove(struct platform_device * pdev)
{
	printk(KERN_ALERT "Goodbye, curel world, this is remove\n");

    return 0;
}

static const struct of_device_id of_inter_dt_match[] = {
	{.compatible = DRIVER_NAME},
	{},
};

MODULE_DEVICE_TABLE(of,of_inter_dt_match);

static struct platform_driver inter_driver = {
	.probe 	= inter_probe,
	.remove = inter_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_inter_dt_match,
		},
};

static int inter_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return platform_driver_register(&inter_driver);
	return 0;
}

static void inter_exit(void)
{
	printk(KERN_ALERT "Goodbye, curel world\n");
	platform_driver_unregister(&inter_driver);
}

module_init(inter_init);
module_exit(inter_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("rty");
MODULE_DESCRIPTION("topeet4412_regiter_dev_drv");
