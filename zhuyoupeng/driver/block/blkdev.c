#include <linux/module.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>


#define RAMBLOCK_SIZE (1024*1024)   			// 1MB��2048����

static struct gendisk *my_ramblock_disk;		// �����豸�Ľṹ��
static struct request_queue *my_ramblock_queue;	// �ȴ�����
static DEFINE_SPINLOCK(my_ramblock_lock);
static int major;
static unsigned char *my_ramblock_buf;			// ������豸���ڴ�ָ��


static void do_my_ramblock_request(struct request_queue *q)
{

	struct request *req;
	static int r_cnt = 0; 			//ʵ���ã���ӡ����������д�ĵ��ȷ���
	static int w_cnt = 0;
	
	req = blk_fetch_request(q);
	
	while (NULL != req)
	{
		unsigned long start = blk_rq_pos(req) *512;
		unsigned long len = blk_rq_cur_bytes(req);
		
		if(rq_data_dir(req) == READ)
		{
			// ������
			memcpy(req->buffer, my_ramblock_buf + start, len); 	//��������
			printk("do_my_ramblock-request read %d times\n", r_cnt++);
		}
		else
		{
			// д����
			memcpy( my_ramblock_buf+start, req->buffer, len); 	//д����
			printk("do_my_ramblock request write %d times\n", w_cnt++);
		}

		if(!__blk_end_request_cur(req, 0)) 
		{
			req = blk_fetch_request(q);
		}
	}
}


static int blk_ioctl(struct block_device *dev, fmode_t no, unsigned cmd, unsigned long arg)
{
	return -ENOTTY;
}

static int blk_open (struct block_device *dev , fmode_t no)
{
	printk("11111blk mount succeed\n");
	return 0;
}
static int blk_release(struct gendisk *gd , fmode_t no)
{
	printk("11111blk umount succeed\n");
	return 0;
}

static const struct block_device_operations my_ramblock_fops =
{
	.owner 		= THIS_MODULE,
	.open 		= blk_open,
	.release 	= blk_release,
	.ioctl 		= blk_ioctl,
};

static int my_ramblock_init(void)
{
	major = register_blkdev(0, "my_ramblock");
	if (major < 0)
	{
		printk("fail to regiser my_ramblock\n");
		return -EBUSY;
	}
	
	// ʵ����
	my_ramblock_disk = alloc_disk(1);		//���豸���� ���������� +1
	
	//��������������У��ṩ��д����
	my_ramblock_queue = blk_init_queue(do_my_ramblock_request, &my_ramblock_lock);
	//����Ӳ������ 
	my_ramblock_disk->major = major;
	my_ramblock_disk->first_minor = 0;
	my_ramblock_disk->fops = &my_ramblock_fops;
	sprintf(my_ramblock_disk->disk_name, "my_ramblcok");		// /dev/name
	my_ramblock_disk->queue = my_ramblock_queue;
	set_capacity(my_ramblock_disk, RAMBLOCK_SIZE / 512);
	/* Ӳ����ز��� */
	my_ramblock_buf = kzalloc(RAMBLOCK_SIZE, GFP_KERNEL);
	add_disk(my_ramblock_disk);				// ���������ע��һ��disk����һ��partation�Ľӿ�
	
	return 0;
}


static void my_ramblock_exit(void)
{
	unregister_blkdev(major, "my_ramblock");
	del_gendisk(my_ramblock_disk);
	put_disk(my_ramblock_disk);
	blk_cleanup_queue(my_ramblock_queue);
	kfree(my_ramblock_buf); 
}

module_init(my_ramblock_init);
module_exit(my_ramblock_exit);

MODULE_LICENSE("GPL"); 
