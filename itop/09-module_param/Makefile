MODULE = module_param

obj-m += $(MODULE)_driver.o

KDIR := /home/topeet/android/iTop4412_Kernel_3.0

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	#arm-linux-gnueabi-gcc $(MODULE)_invoke.c -o $(MODULE)_invoke.o

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers

install:
	cp *.ko /tftpboot/driver/
