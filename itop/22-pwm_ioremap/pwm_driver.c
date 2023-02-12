#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

typedef struct {
	unsigned int	TCFG0;
	unsigned int	TCFG1;
	unsigned int	TCON;
	unsigned int	TCNTB0;
	unsigned int	TCMPB0;
	unsigned int	TCNTO0;
	unsigned int	TCNTB1;
	unsigned int	TCMPB1;
	unsigned int	TCNTO1;
	unsigned int	TCNTB2;
	unsigned int	TCMPB2;
	unsigned int	TCNTO2;
	unsigned int	TCNTB3;
	unsigned int	TCMPB3;
	unsigned int	TCNTO3;
	unsigned int	TCNTB4;
	unsigned int	TCMPB4;
	unsigned int	TCNTO4;
	unsigned int 	TINT_CSTAT;
}PWM_T, *pPWM_t;

pPWM_t pwm;

// save virtual addr and physic addr
volatile unsigned long virt_addr, phys_addr, virt_addr_gpio, phys_addr_gpio;
// sace reg addr
volatile unsigned long *GPD0CON, *GPD0PUD;

static void addr_init(void)
{
	phys_addr = 0x139D0000;
	virt_addr = (unsigned long)ioremap(phys_addr, 0x32);
	pwm = (unsigned long *)(virt_addr + 0x00);

	phys_addr_gpio = 0x11400000 + 0xA0;
	virt_addr_gpio = (unsigned long)ioremap(phys_addr_gpio, 0x10);
	GPD0CON = (unsigned long *)(virt_addr_gpio + 0x00);
	GPD0PUD = (unsigned long *)(virt_addr + 0x08);
}

void pwm_init(void)
{
	addr_init();
	*GPD0CON = *GPD0CON & (~(0xF)) | 0x2;
	*GPD0PUD = *GPD0PUD & (~(0xF));

	// pre scaler: 
	pwm->TCFG0 = pwm->TCFG0 & (~(0xFF)) | 0xF9;
	// frequence
	pwm->TCFG1 = pwm->TCFG1 & (~(0xF)) | 0x2;
	// rate
	pwm->TCNTB0 = 100;
	pwm->TCMPB0 = 20;
	// manual load, start timer
	pwm->TCON =  pwm->TCON & (~(0xF)) | 0x1 | 0x2;
}

void beep_on(void)
{
	pwm->TCON =  pwm->TCON & (~(0xF)) | 0x1 | 0x4 | 0x8;
}

void beep_off(void)
{
	pwm->TCON =  pwm->TCON & (~(0x1));
	*GPD0CON = *GPD0CON & (~(0xF));
}

static int pwm_drv_init(void)
{
	printk(KERN_EMERG "init!\n");
	pwm_init();
	// turn on
	beep_on();

	return 0;
}

static void pwm_drv_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	// turn off
	beep_off();

	return ;
}

module_init(pwm_drv_init);
module_exit(pwm_drv_exit);
