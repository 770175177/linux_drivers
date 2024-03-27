#ifndef __AP3216C_H
#define __AP3216C_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define  AP3216C_IOCTL_BASE     'D'
#define  AP3216C_GET_IR   _IOR(AP3216C_IOCTL_BASE, 0, unsigned short)
#define  AP3216C_GET_ALS  _IOR(AP3216C_IOCTL_BASE, 1, unsigned short)
#define  AP3216C_GET_PS   _IOR(AP3216C_IOCTL_BASE, 2, unsigned short)


#endif
