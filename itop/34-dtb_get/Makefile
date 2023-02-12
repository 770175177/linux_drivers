#!/bin/bash
export ARCH=arm
obj-m += itop4412_register_dev_drv.o 

KDIR := /home/topeet/android/itop4412_kernel_4_14_2_bsp/linux-4.14.2_iTop-4412_scp

PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
		
clean:
	rm *.ko *.o  *.mod.c *.order *.symvers

cp:
	cp *.ko /tftpboot/driver/
