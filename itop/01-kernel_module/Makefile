obj-m += hello_module.o

KDIR := /home/topeet/android/iTop4412_Kernel_3.0

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers

install:
	cp *.ko /tftpboot/driver/
