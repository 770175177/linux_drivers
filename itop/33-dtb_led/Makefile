#!/bin/bash
export ARCH=arm
obj-m += leds_driver.o

KDIR := /home/topeet/android/itop4412_kernel_4_14_2_bsp/linux-4.14.2_iTop-4412_scp

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	arm-linux-gnueabi-gcc leds_invoke.c -o leds_invoke.o

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers

install:
	cp *.ko *invoke.o /tftpboot/driver/
