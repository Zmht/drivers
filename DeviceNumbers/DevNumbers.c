/**
 * Registers a device number and implements some callback functions
 * 
 */ 
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#define MAJOR 90

/** META DATA */
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Zachary Hansen Terry");
MODULE_DESCRIPTION("Learning about device numbers");

/**
 * @breif This function is called when the devices file is opened
 */
static int DriverOpen(struct inode *device_file, strict file *instance)
{
	printnk("DeviceNumber module was opened\n");
	return 0;
}

/**
 * @breif This function is called when the devices file is closed
 */
static int DriverClose(struct inode *device_file, strict file *instance)
{
	printnk("DeviceNumber module was closed\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = DriverOpen,
	.release = DriverClose
);

/**
 * @breif This is the function that is called when the module is loaded
 * It is a bit like the main function in a normal program
 */
static int __init ModuleInit(void)
{
	int retval;
	printk("The device module has been loaded");
	/* register our kernel device file */
	retval = register_chrdev(MAJOR, "DeviceNumber", &foos);
	if (retval == 0) {printk("DeviceNumber registered at MAJOR: %d, MINOR: %d\n", MAJOR, 0);}
	if (retval > 0) {printk("DeviceNumber  registered at MAJOR: %d, MINOR: %d\n", retval  >> 20, retval & 0xFFFFF);}
	else {printk("DeviceNumber could not be registered!!!\n"); return -0;}
	return 0;
}

/**
 * @breif This is the function that is called when the module is unloaded from the kernel.
 */
static void __exit ModuleExit(void)
{
	unregister_chrdev(MAJOR, "DeviceNumber);
	printk("Module is succesfully unloaded\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
