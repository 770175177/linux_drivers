#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define DRIVER_NAME "leds_test"

static int leds_probe(struct platform_device * pdev)
{
	struct pinctrl *leds_pin_ctrl;
	struct pinctrl_state *leds_state;
	int ret;

	printk(KERN_ALERT "ledpinctrl probe init\n");

	leds_pin_ctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(leds_pin_ctrl))
	{
		printk("leds_pin_ctrl failed, %d\n", PTR_ERR(leds_pin_ctrl));
		return -1;
	}

	leds_state = pinctrl_lookup_state(leds_pin_ctrl, "itop-leds1-on");
	if (IS_ERR(leds_state))
	{
		printk("leds_state failed, %d", PTR_ERR(leds_state));
		return -1;
	}

	ret = pinctrl_select_state(leds_pin_ctrl, leds_state);
	if (ret < 0)
	{
		printk("pinctrl_select_state failed\n");
		return -1;
	}

	devm_pinctrl_put(leds_pin_ctrl);

	printk("init ok\n");

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
