#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/leds.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio-exynos4.h>
#include <linux/gpio.h>

static struct led_classdev mydev1;
static struct led_classdev mydev2;

static void itop4412_led1_set(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	printk(KERN_INFO "itop4412_led_set\n");
	
	s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
	
	// use user's setting to control hardware
	// user setting is value
	if(value == LED_OFF)
	{
		// user input 0, turn off
		gpio_set_value(EXYNOS4_GPL2(0), 0);
	}
	else
	{
		// user input not 0, turn on
		gpio_set_value(EXYNOS4_GPL2(0), 1);
	}
}

static void itop4412_led2_set(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	printk(KERN_INFO "itop4412_led_set\n");
	
	s3c_gpio_cfgpin(EXYNOS4_GPK1(1), S3C_GPIO_OUTPUT);
	
	// use user's setting to control hardware
	// user setting is value
	if(value == LED_OFF)
	{
		// user input 0, turn off
		gpio_set_value(EXYNOS4_GPK1(1), 0);
	}
	else
	{
		// user input not 0, turn on
		gpio_set_value(EXYNOS4_GPK1(1), 1);
	}
}

static int __init itop4412_led_init(void)
{
	int ret = -1;

	mydev1.name = "led1";
	mydev1.brightness = 255;
	mydev1.brightness_set = itop4412_led1_set;

	// when insmod, this function will be called
	// use led frame to register a device
	ret = led_classdev_register(NULL, &mydev1);
	if (ret < 0)
	{
		printk(KERN_ERR "led_classdev_register fail\n");
		return ret;
	}

	mydev2.name = "led2";
	mydev2.brightness = 255;
	mydev2.brightness_set = itop4412_led2_set;

	// when insmod, this function will be called
	// use led frame to register a device
	ret = led_classdev_register(NULL, &mydev2);
	if (ret < 0)
	{
		printk(KERN_ERR "led_classdev_register fail\n");
		return ret;
	}

	return 0;
}

static void __exit itop4412_led_exit(void)
{
	led_classdev_unregister(&mydev1);
	led_classdev_unregister(&mydev2);
}

module_init(itop4412_led_init);
module_exit(itop4412_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heng <770175177@qq.com>");
MODULE_DESCRIPTION("itop4412 led driver");
MODULE_ALIAS("itop4412_led");
