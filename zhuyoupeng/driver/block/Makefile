# build driver for host 
#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build

# build driver for EVB
KERN_DIR = /root/kernel/itop4412_kernel_3.0/

CC = arm-none-linux-gnueabi-gcc
#CC = gcc

obj-m   += blkdev.o
all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CC) blk_app.c -o blk_app
	cp *.ko blk_app /tftpboot/driver/

.PHONY: clean
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm blk_app

.PHONY: install
install:
	cp *.ko blk_app /tftpboot/driver/
