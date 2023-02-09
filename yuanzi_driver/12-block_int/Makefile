TARTGET := block_int

KDIR := /home/topeet/yuanzi/kernel
PWD  ?= $(shell pwd)

obj-m += $(TARTGET).o

all:
	make -C $(KDIR) M=$(PWD) modules

PHONY clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers *~

install:
	cp *.ko /tftpboot/yuanzi/
	cp *.ko /nfs/rootfs/root/
