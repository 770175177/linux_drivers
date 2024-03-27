#判断交叉编译工具链是否存在，使用arm-poky-linux-gnueabi- (gcc-5.3.0)
if [ ! -e "/opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi" ]; then
    echo ""
    echo "请先安装正点原子I.MX6U开发板光盘A-基础资料->5、开发工具->1、交叉编译器 
->fsl-imx-x11-glibc-x86_64-meta-toolchain-qt5-cortexa7hf-neon-toolchain-4.1.15-2.1.0.sh"
    echo ""
exit 1
fi

#使用Yocto SDK里的GCC 5.3.0交叉编译器编译出厂Linux源码,可不用指定ARCH等，直接执行Make
source /opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi
#!/bin/bash
#编译前先清除
make distclean
make mx6ull_14x14_ddr256_nand_sd_defconfig
make all -j16
mv u-boot.imx u-boot-imx6ull-14x14-ddr256-nand-sd.imx
mv u-boot.bin u-boot-imx6ull-14x14-ddr256-nand-sd.bin
make mx6ull_14x14_ddr512_nand_sd_defconfig
make all -j16
mv u-boot.imx u-boot-imx6ull-14x14-ddr512-nand-sd.imx
mv u-boot.bin u-boot-imx6ull-14x14-ddr512-nand-sd.bin
make mx6ull_14x14_ddr256_emmc_defconfig
make all -j16
mv u-boot.imx u-boot-imx6ull-14x14-ddr256-emmc.imx
mv u-boot.bin u-boot-imx6ull-14x14-ddr256-emmc.bin
make mx6ull_14x14_ddr512_emmc_defconfig
make all -j16
mv u-boot.imx u-boot-imx6ull-14x14-ddr512-emmc.imx
mv u-boot.bin u-boot-imx6ull-14x14-ddr512-emmc.bin
make mx6ull_14x14_ddr256_nand_defconfig
make all -j16
mv u-boot.imx u-boot-imx6ull-14x14-ddr256-nand.imx
mv u-boot.bin u-boot-imx6ull-14x14-ddr256-nand.bin
make mx6ull_14x14_ddr512_nand_defconfig
make all -j16
mv u-boot.imx u-boot-imx6ull-14x14-ddr512-nand.imx
mv u-boot.bin u-boot-imx6ull-14x14-ddr512-nand.bin
#在当前目录下新建一个tmp目录，用于存放编译后的目标文件
if [ ! -e "./tmp" ]; then
    mkdir tmp
fi
rm -rf tmp/*
#拷贝所有编译的U-boot.imx及U-boot.bin到当前的tmp目录下
mv u-boot-imx6ull*.bin tmp
mv u-boot-imx6ull*.imx tmp
echo "编译完成，请查看当前目录下的tmp文件夹查看编译好的目标文件"
