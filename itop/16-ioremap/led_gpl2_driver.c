#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

// save virtual addr and physic addr
volatile unsigned long virt_addr, phys_addr;
// sace reg addr
volatile unsigned long *GPL2CON, *GPL2DAT, *GPL2PUD;

void gpl2_device_init(void)
{
	// start address: 0x11000100 -> 0x11000108
	phys_addr = 0x11000100;
	// 0x11000100 is GPL2CON physic address
	virt_addr = (unsigned long)ioremap(phys_addr, 0x10);
	// point to address that need to operate
	GPL2CON = (unsigned long *)(virt_addr + 0x00);
	GPL2DAT = (unsigned long *)(virt_addr + 0x04);
	GPL2PUD = (unsigned long *)(virt_addr + 0x08);
}

void gpl2_configure(void)
{
	// output, bit 3:1 -> 0 | bit 0 -> 1
	*GPL2CON &= 0xfffffff0;
	*GPL2CON |= 0x00000001;
	// pud, bit 1:0 -> 0x3
	*GPL2PUD |= 0x0003;
}

void gpl2_on(void)
{
	*GPL2DAT |= 0x01;
}

void gpl2_off(void)
{
	*GPL2DAT &= 0xfe;
}

static int led_gpl2_init(void)
{
	printk(KERN_EMERG "init!\n");
	// map virtual mem to physic mem
	gpl2_device_init();
	// config gpl2 to output mode
	gpl2_configure();
	// turn on
	gpl2_on();
	printk(KERN_EMERG "turn led gpl2\n");

	return 0;
}

static void led_gpl2_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");
	// turn off
	gpl2_off();

	return ;
}

module_init(led_gpl2_init);
module_exit(led_gpl2_exit);
