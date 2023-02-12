obj-m += leds_driver.o

KDIR := /home/topeet/android/iTop4412_Kernel_3.0

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	arm-linux-gnueabi-gcc leds_invoke.c -o leds_invoke.o

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers

install:
	cp *.ko *invoke.o /tftpboot/driver/
