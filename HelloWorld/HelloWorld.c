#include <linux/kernel.h>
#include <linux/module.h>

/** META DATA */
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Zachary Hansen Terry");
MODULE_DESCRIPTION("A Simple Hello World Module");

/**
 * @breif This is the function that is called when the module is loaded
 * It is a bit like the main function in a normal program
 */
static int __init ModuleInit(void)
{
	printk("Hello Kernel\n");
	return 0;
}

/**
 * @breif This is the function that is called when the module is unloaded from the kernel.
 */
static void __exit ModuleExit(void)
{
	printk("Module is succesfully unloaded\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
