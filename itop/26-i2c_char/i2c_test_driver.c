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
#include <asm/uaccess.h>
#include <linux/miscdevice.h>

/* NOTE: support mulititouch only */
#ifdef CONFIG_STAGING	//in linux Qt single report
#define CONFIG_FT5X0X_MULTITOUCH		1
#endif

#define FT5X0X_NAME     "i2c_test"

MODULE_AUTHOR("heng");
MODULE_DESCRIPTION("TS I2C TEST");
MODULE_LICENSE("GPL");

static struct i2c_client *this_client;

static int i2c_test_read_reg(struct i2c_client *client, unsigned char addr, unsigned char *pdata)
{
	unsigned char buf1[4] = {0};
	unsigned char buf2[4] = {0};
	struct i2c_msg msgs[] = {
		{
			.addr  = client->addr,
			.flags = 0,
			.len   = 1,
			.buf    = buf1,
		},
		{
			.addr  = client->addr,
			.flags = I2C_M_RD,
			.len   = 1,
			.buf    = buf2,
		}
	};
	int ret;
	
	buf1[0] = addr;
	ret = i2c_transfer(client->adapter, msgs, 2);
	if (ret < 0)
	{
		printk("read_reg (0x%02x) error, ret %d\n", addr, ret);
	}
	else
	{
		*pdata = buf2[0];
	}

	return ret;
}

static int i2c_test_read_fw_reg(struct i2c_client *client, unsigned char *val)
{
	int ret;

	*val = 0xff;
	ret = i2c_test_read_reg(client, 0xA6, val);

	printk(KERN_ERR "ts reg 0xA6 val is 0x%x\n", *val);

	return ret;
}

static int i2c_open_func(struct file *filp)
{

	return 0;
}

static int i2c_release_func(struct file *filp)
{

	return 0;
}

static size_t i2c_read_func(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
	int ret;
	unsigned char reg_data;

	ret = copy_from_user(reg_data, buf, 1);

	struct i2c_msg msgs[] = {
		{
			.addr  = this_client->addr,
			.flags = 0,
			.len   = 1,
			.buf   = &reg_data,
		},
		{
			.addr  = this_client->addr,
			.flags = I2C_M_RD,
			.len   = 1,
			.buf   = &reg_data,
		},
	};

	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
	{
		printk(KERN_ERR "read reg error, ret %d\n", ret);
	}
	
	ret = copy_to_user(buf, &reg_data, 1);

	return 0;
}

static size_t i2c_write_func(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
	int ret;
	unsigned char buf1[2];

	ret = copy_from_user(buf1, buf, 1);

	struct i2c_msg msgs[1];
	msgs[0].addr  = this_client->addr,
	msgs[0].flags = 0,
	msgs[0].len   = 1,
	msgs[0].buf   = buf1,

	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
	{
		printk(KERN_ERR "write reg error, ret %d\n", ret);
	}
	
	ret = copy_to_user(buf, buf1, 1);

	return 0;
}

static struct file_operation i2c_ops = {
	.owner = THIS_MODULE,
	.open  = i2c_open_func,
	.release = i2c_release_func,
	.write = i2c_write_func,
	.read  = i2c_read_func,
};

static struct miscdevice i2c_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops  = &i2c_ops,
	.name  = "i2c_control",
};

static int i2c_test_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	unsigned char val;

	printk("--%s--\n", __FUNCTION__);	

	i2c_test_read_fw_reg(client, &val);

	this_client = client;

	ret = misc_register(&i2c_dev);

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

