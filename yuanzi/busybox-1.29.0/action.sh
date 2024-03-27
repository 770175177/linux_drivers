#!/bin/bash

if [[ -n $1 ]] && [[ $1 == "clean" ]]; then

	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean

	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
fi

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j6

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install CONFIG_PREFIX=/home/heng/nfsroot
