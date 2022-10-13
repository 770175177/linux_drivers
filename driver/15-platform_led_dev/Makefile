TARTGET := platform_led_driver

KDIR := /home/topeet/yuanzi/kernel
PWD  ?= $(shell pwd)

obj-m += $(TARTGET).o platform_led_device.o

all:
	make -C $(KDIR) M=$(PWD) modules

PHONY clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers *~

install:
	cp *.ko /tftpboot/yuanzi/
	cp *.ko /nfs/rootfs/root/
