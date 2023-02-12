#include <linux/init.h>
#include <linux/module.h>
/*define module_param module_param_array*/
#include <linux/moduleparam.h>
/*define parameter of module_param and module_param_array*/
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HengLiang");

static int module_arg1, module_arg2;
static int int_array[50];
static int int_num;

module_param(module_arg1, int, S_IRUSR);

module_param(module_arg2, int, S_IRUSR);

module_param_array(int_array, int, &int_num, S_IRUSR);

static int hello_init(void)
{
	int i;

	printk(KERN_EMERG "module arg1 is %d!\n", module_arg1);
	printk(KERN_EMERG "module arg1 is %d!\n", module_arg2);
	for(i = 0; i < int_num; i++)
	{
		printk(KERN_EMERG "int_array[%d] is %d!\n", i, int_array[i]);
	}

	printk(KERN_EMERG "Hello World Enter!\n");
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_EMERG "Hello World exit!\n");

	return ;
}

module_init(hello_init);
module_exit(hello_exit);
