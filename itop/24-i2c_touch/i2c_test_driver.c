#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include <linux/regulator/consumer.h>

#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
//#include <plat/ctouch.h>

/* NOTE: support mulititouch only */
#ifdef CONFIG_STAGING	//in linux Qt single report
#define CONFIG_FT5X0X_MULTITOUCH		1
#endif

#define FT5X0X_NAME     "i2c_test"

MODULE_AUTHOR("heng");
MODULE_DESCRIPTION("TS I2C TEST");
MODULE_LICENSE("GPL");

static int i2c_test_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("--%s--\n", __FUNCTION__);	

	return 0;
}

static int __devexit i2c_test_remove(struct i2c_client *client) 
{
	printk("--%s--\n", __FUNCTION__);	
	i2c_set_clientdata(client, NULL);

	return 0;
}

static const struct i2c_device_id i2c_test_id[] = {
	{ FT5X0X_NAME, 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, i2c_test_id);

static struct i2c_driver i2c_test_driver = {
	.probe		= i2c_test_probe,
	.remove		= __devexit_p(i2c_test_remove),
	.id_table	= i2c_test_id,
	.driver	= {
		.name	= FT5X0X_NAME,
		.owner	= THIS_MODULE,
	},
};

static void i2c_io_init(void)
{
	int ret = 0;

	ret = gpio_request(EXYNOS4_GPL0(2), "TP1_EN");
	if (ret)
	{
		printk(KERN_ERR "failed to request TP1_EN for i2c control\n");
	}

	gpio_direction_output(EXYNOS4_GPL0(2), 1);

	s3c_gpio_cfgpin(EXYNOS4_GPL0(2), S3C_GPIO_OUTPUT);
	gpio_free(EXYNOS4_GPL0(2));
	mdelay(5);

	ret = gpio_request(EXYNOS4_GPX0(3), "GPX0_3");
	if (ret)
	{
		gpio_free(EXYNOS4_GPX0(3));
		ret = gpio_request(EXYNOS4_GPX0(3), "GPX0_3");
		if (ret)
		{
			printk(KERN_ERR "failed to request GPX0_3\n");
		}
	}

	gpio_direction_output(EXYNOS4_GPX0(3), 0);
	mdelay(200);

	gpio_direction_output(EXYNOS4_GPX0(3), 1);

	s3c_gpio_cfgpin(EXYNOS4_GPX0(3), S3C_GPIO_OUTPUT);
	gpio_free(EXYNOS4_GPX0(3));
	mdelay(300);
}

static int __init i2c_test_init(void)
{
	printk("--%s--\n", __FUNCTION__);	

	i2c_io_init();

	return i2c_add_driver(&i2c_test_driver);
}

static void __exit i2c_test_exit(void)
{
	printk("--%s--\n", __FUNCTION__);	

	i2c_del_driver(&i2c_test_driver);
}

late_initcall(i2c_test_init);
module_exit(i2c_test_exit);

