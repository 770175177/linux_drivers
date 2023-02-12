#!/bin/bash
export ARCH=arm
MOD    = pinctrl_dtb_irq
obj-m += $(MOD).o 

KDIR := /home/topeet/android/itop4412_kernel_4_14_2_bsp/linux-4.14.2_iTop-4412_scp

PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	arm-none-linux-gnueabi-gcc -o $(MOD)_invoke.o $(MOD)_invoke.c
		
clean:
	rm *.ko *.o  *.mod.c *.order *.symvers

cp:
	cp *.ko $(MOD)_invoke.o /tftpboot/driver/
