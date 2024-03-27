#!/bin/bash

if [[ -n $1 ]] && [[ $1 == "clean" ]]; then

	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean

	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
fi

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j6

#make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install CONFIG_PREFIX=/home/heng/nfsroot

cp output/images/rootfs.tar /home/heng/nfsroot/buildrootfs/ 

cd /home/heng/nfsroot/buildrootfs/

cp ./etc/profile ./etc/profile.bak

tar -xvf rootfs.tar

mv ./etc/profile.bak ./etc/profile

cd -


# 如果编译不过，需要使用su root，然后进行编译
