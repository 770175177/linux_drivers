#!/bin/bash

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- mx6ull_14x14_ddr256_nand_sd_defconfig 

make V=0 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j14
