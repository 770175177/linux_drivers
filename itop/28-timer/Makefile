obj-m += my_time.o

KDIR := /home/topeet/android/iTop4412_Kernel_3.0

PWD := $(shell pwd) 

all:  
	$(MAKE) -C $(KDIR) M=$(PWD) modules  
	
clean:  
	rm *.mod.c *.o *.ko  modules.* Module

install:
	cp *.ko /tftpboot/driver/
