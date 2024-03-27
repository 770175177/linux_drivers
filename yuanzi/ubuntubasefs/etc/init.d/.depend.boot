TARGETS = hostname.sh mountkernfs.sh mountdevsubfs.sh procps hwclock.sh checkroot.sh urandom mountall.sh checkfs.sh checkroot-bootclean.sh mountnfs-bootclean.sh mountnfs.sh bootmisc.sh mountall-bootclean.sh
INTERACTIVE = checkroot.sh checkfs.sh
mountdevsubfs.sh: mountkernfs.sh
procps: mountkernfs.sh
hwclock.sh: mountdevsubfs.sh
checkroot.sh: hwclock.sh mountdevsubfs.sh hostname.sh
urandom: hwclock.sh
mountall.sh: checkfs.sh checkroot-bootclean.sh
checkfs.sh: checkroot.sh
checkroot-bootclean.sh: checkroot.sh
mountnfs-bootclean.sh: mountnfs.sh
bootmisc.sh: mountnfs-bootclean.sh mountall-bootclean.sh checkroot-bootclean.sh
mountall-bootclean.sh: mountall.sh
