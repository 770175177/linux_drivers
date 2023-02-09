
/* �ο� 
 * drivers\mtd\nand\s3c2410.c
 * drivers\mtd\nand\at91_nand.c
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/clk.h>
 
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
 
#include <asm/io.h>
 
#include <asm/arch/regs-nand.h>
#include <asm/arch/nand.h>


static struct nand_chip *s3c_nand;
static struct mtd_info *s3c_mtd;

static void s3c2440_select_chip(struct mtd_info *mtd, int chipnr)
{
	if (chipnr == -1)
	{
		/* ȡ��ѡ��: NFCONT[1]��Ϊ0 */
	}
	else
	{
		/* ѡ��: NFCONT[1]��Ϊ1 */
		
	}
}

static void s3c2440_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	if (ctrl & NAND_CLE)
	{
		/* ������: NFCMMD=dat */
	}
	else
	{
		/* ����ַ: NFADDR=dat */
	}
}

static int s3c2440_dev_ready(struct mtd_info *mtd)
{
	return "NFSTAT��bit[0]";
}


static int s3c_nand_init(void)
{
	/* 1. ����һ��nand_chip�ṹ�� */
	s3c_nand = kzalloc(sizeof(struct nand_chip), GFP_KERNEL);
	
	/* 2. ����nand_chip */
	/* ����nand_chip�Ǹ�nand_scan����ʹ�õ�, �����֪����ô����, �ȿ�nand_scan��ôʹ�� 
	 * ��Ӧ���ṩ:ѡ��,������,����ַ,������,������,�ж�״̬�Ĺ���
	 */
	s3c_nand->select_chip = s3c2440_select_chip;
	s3c_nand->cmd_ctrl    = s3c2440_cmd_ctrl;
	s3c_nand->IO_ADDR_R   = "NFDATA�������ַ";
	s3c_nand->IO_ADDR_W   = "NFDATA�������ַ";
	s3c_nand->dev_ready   = s3c2440_dev_ready;
	
	/* 3. Ӳ����ص����� */
	
	/* 4. ʹ��: nand_scan */
	s3c_mtd = kzalloc(sizeof(struct mtd_info), GFP_KERNEL);
	s3c_mtd->owner = THIS_MODULE;
	s3c_mtd->priv  = s3c_nand;
	
	nand_scan(s3c_mtd, 1);  /* ʶ��NAND FLASH, ����mtd_info */
	
	/* 5. add_mtd_partitions */
	
	return 0;
}

static void s3c_nand_exit(void)
{
}

module_init(s3c_nand_init);
module_exit(s3c_nand_exit);

MODULE_LICENSE("GPL");

