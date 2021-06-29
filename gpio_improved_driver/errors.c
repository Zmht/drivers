#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#include "errors.h"

void GpioError(int pinNum,)
{
    gpio_free(pinNum);
    AddError();
}

void AddError()
{
    device_destroy(my_class, *my_device_nr);
    FileError();
}

void FileError()
{
    class_destroy(my_class);
    ClassError();
}

void ClassError()
{
    unregister_chrdev(*my_device_nr, DRIVER_NAME);
}