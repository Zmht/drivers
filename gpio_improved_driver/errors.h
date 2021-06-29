#pragma once

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

void GpioError(int pinNum);
void AddError();
void FileError();
void ClassError();