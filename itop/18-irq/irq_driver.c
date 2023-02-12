#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>

#include <linux/interrupt.h>
#include <linux/irq.h>

#define DRIVER_NAME "irq_test"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static irqreturn_t eint9_interrupt(int irq, void *dev_id)
{
	printk("%s(%d)\n", __FUNCTION__, __LINE__);

	return IRQ_HANDLED;
}

static irqreturn_t eint10_interrupt(int irq, void *dev_id)
{
	printk("%s(%d)\n", __FUNCTION__, __LINE__);

	return IRQ_HANDLED;
}

static int irq_probe (struct platform_device *pdev)
{
	int ret;
	char *banner = "irq_test Initialize\n";

	printk(banner);

	ret =  gpio_request(EXYNOS4_GPL1(1), "EINT9");
	if ( ret < 0 )
	{
		printk(KERN_EMERG "gpio_request EXYNOS4_GPL1(1) failed\n");
		return ret;
	}
	s3c_gpio_cfgpin(EXYNOS4_GPL1(1), S3C_GPIO_SFN(0xF));	
	s3c_gpio_setpull(EXYNOS4_GPL1(1), S3C_GPIO_PULL_UP);
	gpio_free(EXYNOS4_GPL1(1));

	ret = request_irq(IRQ_EINT(9), eint9_interrupt, IRQ_TYPE_EDGE_FALLING, "eint9", pdev);

	ret =  gpio_request(EXYNOS4_GPL1(2), "EINT10");
	if ( ret < 0 )
	{
		printk(KERN_EMERG "gpio_request EXYNOS4_GPL1(2) failed\n");
		return ret;
	}
	s3c_gpio_cfgpin(EXYNOS4_GPL1(2), S3C_GPIO_SFN(0xF));	
	s3c_gpio_setpull(EXYNOS4_GPL1(2), S3C_GPIO_PULL_UP);
	gpio_free(EXYNOS4_GPL1(2));

	ret = request_irq(IRQ_EINT(10), eint10_interrupt, IRQ_TYPE_EDGE_FALLING, "eint10", pdev);

	return 0;

//exit:
//	return ret;
}

static int irq_remove (struct platform_device *pdev)
{
	free_irq(IRQ_EINT(9), pdev);
	free_irq(IRQ_EINT(10), pdev);
	
	return 0;
}

static int irq_suspend (struct platform_device *pdev, pm_message_t state)
{
	printk(KERN_EMERG "irq suspend: power off!\n");

	return 0;
}

static int irq_resume (struct platform_device *pdev)
{
	printk(KERN_EMERG "irq resume: power on!\n");

	return 0;
}

static struct platform_driver irq_driver = {
	.probe = irq_probe,
	.remove = irq_remove,
	.suspend = irq_suspend,
	.resume = irq_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init irq_test_init(void)
{
	//printk(KERN_EMERG "turn led irq_test\n");

	return platform_driver_register(&irq_driver);
}

static void irq_test_exit(void)
{
	//printk(KERN_EMERG "Hello World exit!\n");
	platform_driver_unregister(&irq_driver);
}

module_init(irq_test_init);
module_exit(irq_test_exit);
