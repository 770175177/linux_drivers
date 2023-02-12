# build driver for host 
#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build

# build driver for EVB
KERN_DIR = /root/kernel/itop4412_kernel_3.0/

CC = arm-none-linux-gnueabi-gcc
#CC = gcc

obj-m   += leds-itop4412.o
all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CC) app.c -o app
	cp *.ko app /tftpboot/driver/

.PHONY: clean
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm app

.PHONY: install
install:
	cp *.ko app /tftpboot/driver/
