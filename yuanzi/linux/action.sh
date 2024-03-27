#!/bin/bash

if [ "$1" == "clean" ]; then
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- imx_v7_defconfig 
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig 
fi

make V=0 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- all -j16

cp arch/arm/boot/zImage /home/heng/tftp/zImage_alientek

cp arch/arm/boot/dts/imx6ull-alientek-emmc.dtb /home/heng/tftp/

cd /home/heng/tftp/

ln -sf zImage_alientek zImage
ln -sf imx6ull-alientek-emmc.dtb imx6ull.dtb

cd -
