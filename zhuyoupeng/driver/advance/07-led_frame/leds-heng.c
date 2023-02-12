#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/leds.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio-exynos4.h>
#include <linux/gpio.h>

static struct led_classdev mydev;

static void itop4412_led_set(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	printk(KERN_INFO "itop4412_led_set\n");
	// use user's setting to control hardware
	// user setting is value
	if(value == LED_OFF)
	{
		// user input 0, turn off
		s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
		gpio_set_value(EXYNOS4_GPL2(0), 0);
	}
	else
	{
		// user input not 0, turn on
		s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
		gpio_set_value(EXYNOS4_GPL2(0), 1);
	}
}

static int __init itop4412_led_init(void)
{
	int ret = -1;

	mydev.name = "leds-heng";
	mydev.brightness = 0;
	mydev.brightness_set = itop4412_led_set;

	// when insmod, this function will be called
	// use led frame to register a device
	ret = led_classdev_register(NULL, &mydev);
	if (ret < 0)
	{
		printk(KERN_ERR "led_classdev_register fail\n");
		return ret;
	}

	return 0;
}

static void __exit itop4412_led_exit(void)
{
	led_classdev_unregister(&mydev);
}

module_init(itop4412_led_init);
module_exit(itop4412_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heng <770175177@qq.com>");
MODULE_DESCRIPTION("itop4412 led driver");
MODULE_ALIAS("itop4412_led");
