#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>

#include <asm/mach/map.h>
#include <asm/arch/regs-lcd.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/fb.h>


static struct fb_info *s3c_lcd;

static int lcd_init(void)
{
	/* 1. ����һ��fb_info */
	s3c_lcd = framebuffer_alloc(0, NULL);

	/* 2. ���� */
	/* 2.1 ���ù̶��Ĳ��� */
	/* 2.2 ���ÿɱ�Ĳ��� */
	/* 2.3 ���ò������� */
	/* 2.4 ���������� */

	/* 3. Ӳ����صĲ��� */
	/* 3.1 ����GPIO����LCD */
	/* 3.2 ����LCD�ֲ�����LCD������, ����VCLK��Ƶ�ʵ� */
	/* 3.3 �����Դ�(framebuffer), ���ѵ�ַ����LCD������ */

	/* 4. ע�� */
	register_framebuffer(s3c_lcd);
	
	return 0;
}

static void lcd_exit(void)
{
}

module_init(lcd_init);
module_exit(lcd_exit);

MODULE_LICENSE("GPL");


