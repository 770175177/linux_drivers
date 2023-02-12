# build driver for host 
#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build

# build driver for EVB
KERN_DIR = /root/kernel/itop4412_kernel_3.0/

obj-m   += hello.o
all:
	make -C $(KERN_DIR) M=`pwd` modules

.PHONY: clean
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean

.PHONY: install
install:
	cp *.ko /tftpboot/driver/
