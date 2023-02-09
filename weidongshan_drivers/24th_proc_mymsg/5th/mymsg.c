#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/proc_fs.h>

#define MYLOG_BUF_LEN 1024

struct proc_dir_entry *myentry;

static char mylog_buf[MYLOG_BUF_LEN];
static char tmp_buf[MYLOG_BUF_LEN];
static int mylog_r = 0;
static int mylog_w = 0;

static DECLARE_WAIT_QUEUE_HEAD(mymsg_waitq);

static int is_mylog_empty(void)
{
	return (mylog_r == mylog_w);
}

static int is_mylog_full(void)
{
	return ((mylog_w + 1)% MYLOG_BUF_LEN == mylog_r);
}

static void mylog_putc(char c)
{
	if (is_mylog_full())
	{
		/* ����һ������ */
		mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;
	}

	mylog_buf[mylog_w] = c;
	mylog_w = (mylog_w + 1) % MYLOG_BUF_LEN;

	/* ���ѵȴ����ݵĽ��� */	
    wake_up_interruptible(&mymsg_waitq);   /* �������ߵĽ��� */	
}

static int mylog_getc(char *p)
{
	if (is_mylog_empty())
	{
		return 0;
	}
	*p = mylog_buf[mylog_r];
	mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;
	return 1;
}

int myprintk(const char *fmt, ...)
{
	va_list args;
	int i;
	int j;

	va_start(args, fmt);
	i = vsnprintf(tmp_buf, INT_MAX, fmt, args);
	va_end(args);
	
	for (j = 0; j < i; j++)
		mylog_putc(tmp_buf[j]);
		
	return i;
}

static ssize_t mymsg_read(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	int error = 0;
	int i = 0;
	char c;

	/* ��mylog_buf������copy_to_user, return */
	if ((file->f_flags & O_NONBLOCK) && is_mylog_empty())
		return -EAGAIN;

	//printk("%s %d\n", __FUNCTION__, __LINE__);
	//printk("count = %d\n", count);
	//printk("mylog_r = %d\n", mylog_r);
	//printk("mylog_w = %d\n", mylog_w);

	error = wait_event_interruptible(mymsg_waitq, !is_mylog_empty());

	//printk("%s %d\n", __FUNCTION__, __LINE__);
	//printk("count = %d\n", count);
	//printk("mylog_r = %d\n", mylog_r);
	//printk("mylog_w = %d\n", mylog_w);

	/* copy_to_user */
	while (!error && (mylog_getc(&c)) && i < count) {
		error = __put_user(c, buf);
		buf++;
		i++;
	}
	
	if (!error)
		error = i;
	
	return error;
}

const struct file_operations proc_mymsg_operations = {
	.read = mymsg_read,
};

static int mymsg_init(void)
{	
	myentry = create_proc_entry("mymsg", S_IRUSR, &proc_root);
	if (myentry)
		myentry->proc_fops = &proc_mymsg_operations;
	return 0;
}

static void mymsg_exit(void)
{
	remove_proc_entry("mymsg", &proc_root);
}

module_init(mymsg_init);
module_exit(mymsg_exit);

EXPORT_SYMBOL(myprintk);

MODULE_LICENSE("GPL");

