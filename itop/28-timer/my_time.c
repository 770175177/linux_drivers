#include "linux/module.h"
#include "linux/timer.h"
#include "linux/jiffies.h"

struct timer_list demo_timer;

static void time_func(unsigned long data)
{
	printk("%s ,secs = %ld!\n",(char *)data,jiffies/HZ);
	
	mod_timer(&demo_timer,jiffies + 5*HZ);
}

static int __init mytimer_init(void)
{
	printk("mytimer_init!\n");
	setup_timer(&demo_timer,time_func,(unsigned long) "demo_timer!");
	demo_timer.expires = jiffies + 1*HZ;
	add_timer(&demo_timer);
	
	return 0;
}

static void __exit mytimer_exit(void)
{
	printk("mytimer_exit!\n");
	del_timer(&demo_timer);
}

module_init(mytimer_init);
module_exit(mytimer_exit);

MODULE_LICENSE("Dual BSD/GPL");