#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/leds.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio-exynos4.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>

#define GPIO_LED1	EXYNOS4_GPL2(0)
#define LED_OFF		0
#define LED_ON		1

static struct led_classdev mydev1;

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

static int itop4412_led_probe(struct platform_device *dev)
{
	int ret = -1;

	printk(KERN_INFO "----itop4412----\n");

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


	return 0;
}

static int itop4412_led_remove(struct platform_device *dev)
{
	led_classdev_unregister(&mydev1);

	return 0;
}

static struct platform_driver itop4412_led_driver = {
	.probe		= itop4412_led_probe,
	.remove		= itop4412_led_remove,
	.driver		= {
		.name		= "itop4412_led",
		.owner		= THIS_MODULE,
	},
};

struct itop4412_led_platdata {
	unsigned int		 gpio;
	unsigned int		 flags;

	char			*name;
	char			*def_trigger;
};

// LED platform device
#define ITOP4412_LEDF_ACTLOW	(1<<0)		/* LED is on when GPIO low */
#define ITOP4412_LEDF_TRISTATE	(1<<1)		/* tristate to turn off */
static struct itop4412_led_platdata itop4412_led1_pdata = {
	.name		= "led1",
	.gpio		= EXYNOS4_GPL2(0),
	.flags		= ITOP4412_LEDF_ACTLOW | ITOP4412_LEDF_TRISTATE,
	.def_trigger	= "heartbeat",
};

static struct platform_device itop4412_led1 = {
	.name		= "itop4412_led",
	.id		= 1,
	.dev		= {
		.platform_data	= &itop4412_led1_pdata,
	},
};

static int __init itop4412_led_init(void)
{
	return platform_driver_register(&itop4412_led_driver);
}

static void __exit itop4412_led_exit(void)
{
	platform_driver_unregister(&itop4412_led_driver);
}

module_init(itop4412_led_init);
module_exit(itop4412_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heng <770175177@qq.com>");
MODULE_DESCRIPTION("itop4412 led driver");
MODULE_ALIAS("itop4412_led");
