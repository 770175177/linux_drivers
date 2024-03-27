#判断交叉编译工具链是否存在，使用arm-poky-linux-gnueabi- (gcc-5.3.0)
if [ ! -e "/opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi" ]; then
    echo ""
    echo "请先安装正点原子I.MX6U开发板光盘A-基础资料->5、开发工具->1、交叉编译器 
->fsl-imx-x11-glibc-x86_64-meta-toolchain-qt5-cortexa7hf-neon-toolchain-4.1.15-2.1.0.sh"
    echo ""
fi

#使用Yocto SDK里的GCC 5.3.0交叉编译器编译出厂Linux源码,可不用指定ARCH等，直接执行Make
source /opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi
#!/bin/bash
#编译前先清除
make distclean
#配置defconfig文件
make imx_v7_defconfig -j 16
#开始编译zImage
make zImage -j 16
#eMMC类型核心板
#编译正点原子各种显示设备的设备树，若用户没有屏，
#启动时默认会选择imx6ull-14x14-emmc-4.3-480x272-c.dtb加载
make imx6ull-14x14-emmc-10.1-1280x800-c.dtb
make imx6ull-14x14-emmc-7-1024x600-c.dtb
make imx6ull-14x14-emmc-7-800x480-c.dtb
make imx6ull-14x14-emmc-4.3-800x480-c.dtb
make imx6ull-14x14-emmc-4.3-480x272-c.dtb
make imx6ull-14x14-emmc-hdmi.dtb
make imx6ull-14x14-emmc-vga.dtb

#NandFlash类型核心板
#编译正点原子各种显示设备的设备树，若用户没有屏，
#启动时默认会选择imx6ull-14x14-nand-4.3-480x272-c.dtb加载
make imx6ull-14x14-nand-10.1-1280x800-c.dtb
make imx6ull-14x14-nand-7-1024x600-c.dtb
make imx6ull-14x14-nand-7-800x480-c.dtb
make imx6ull-14x14-nand-4.3-800x480-c.dtb
make imx6ull-14x14-nand-4.3-480x272-c.dtb
make imx6ull-14x14-nand-vga.dtb
make imx6ull-14x14-nand-hdmi.dtb
#编译内核模块
make modules -j 16
#在当前目录下新建一个tmp目录，用于存放编译后的目标文件
if [ ! -e "./tmp" ]; then
    mkdir tmp
fi
rm -rf tmp/*
make modules_install INSTALL_MOD_PATH=tmp
cd tmp/lib/modules
tar -jcvf ../../modules.tar.bz2 .
cd -
rm -rf tmp/lib
#拷贝zImage到tmp目录下
cp arch/arm/boot/zImage tmp
#拷贝所有编译的设备树文件到当前的tmp目录下
cp arch/arm/boot/dts/imx6ull*.dtb tmp
echo "编译完成，请查看当前目录下的tmp文件夹查看编译好的目标文件"
