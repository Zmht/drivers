#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#include "errors.h"

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zachary Hansen Terry");
MODULE_DESCRIPTION("A simple GPIO driver for setting an LED and reading a button");


/** Variable for device and device class */
dev_t my_device_nr;
struct class *my_class;
struct cdev my_device;

#define DRIVER_NAME "gpio_driver"
#define DRIVER_CLASS "MyModuleClass"

/**
 * @brief Reads data out of the buffer
 */
static ssize_t driver_read(struct file *filep, char* user_buffer, size_t count, loff_t *offset)
{
	int to_copy, not_copied, delta;
	char tmp[3] = "\n";

	/* get amount of data to copy */
	to_copy = min(count, sizeof(tmp));

	/** Read value of button */
	printk("Value of button: %d\n", gpio_get_value(17));
	tmp[0] = gpio_get_value(17) + '0';

	/** Copy data to the user */
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);

	/** Calculate data */
	delta = to_copy - not_copied;

	return delta;
}

/** 
 * @brief Write data to the buffer
 */
static ssize_t driver_write(struct file *filep, const char* user_buffer, size_t count, loff_t *offset)
{
	int to_copy, not_copied, delta;
	char value;

	/* get amount of data to copy */
	to_copy = min(count, sizeof(value));

	/** Copy data to the user */
	not_copied = copy_from_user(&value, user_buffer, to_copy);

	switch (value)
	{
	case '0':
		gpio_set_value(4, 0);
		break;
	case '1':
		gpio_set_value(4, 1);
		break;
	default:
		printk("gpio_driver - Invalid input\n");
		break;
	}

	/** Calculate data */
	delta = to_copy - not_copied;

	return delta;
}


/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("gpio_driver - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("gpio_driver - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};


/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("gpio_driver - Hello, Kernel!\n");

	/** Allocate a device nr */
	if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0)
	{
		printk("gpio_driver - Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("gpio_driver - Device Nr. Major: %d was registered", my_device_nr >> 20);

	/** Create a device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
	{
		printk("gpio_driver - Device class can not be created\n");
		ClassError();
		return -1;
	}

	/** Create device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) ==  NULL)
	{
		printk("gpio_driver - Cannot create device file!\n");
		FileError();
		return -1;
	}

	/** Init device file */
	cdev_init(&my_device, &fops);

	if(cdev_add(&my_device, my_device_nr, 1) == -1)
	{
		printk("gpio_driver - Registering device of kernel failedn\n");
		AddError();
		return -1;
	}

	/** GPIO 4 init */
	if(gpio_request(4, "rpi-gpio-4"))
	{
		printk("gpio-driver - Cannot allocate gpio pin 4\n");
		AddError();
		return -1;
	}

	/** Set GPIO 4 direction */
	if(gpio_direction_output(4, 0))
	{
		printk("gpio_driver - cannot set gpio 4 to output\n");
		GpioError(4);
		return -1;
	}

	/** GPIO 17 init */
	if(gpio_request(17, "rpi-gpio-17"))
	{
		printk("gpio-driver - Cannot allocate gpio pin 17\n");
		GpioError(17);
		return -1;
	}

	/** Set GPIO 4 direction */
	if(gpio_direction_input(17))
	{
		printk("gpio_driver - cannot set gpio 17 to input\n");
		GpioError(17);
		return -1;
	}


	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	gpio_set_value(4, 0);
	gpio_free(4);
	gpio_free(17);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev(my_device_nr, DRIVER_NAME);
	printk("gpio_driver - Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

