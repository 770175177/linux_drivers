MOD = pwm

obj-m += $(MOD)_driver.o

KDIR := /home/topeet/android/iTop4412_Kernel_3.0

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	arm-linux-gnueabi-gcc $(MOD)_invoke.c -o $(MOD)_invoke.o

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers

install:
	cp *.ko *invoke.o /tftpboot/driver/
