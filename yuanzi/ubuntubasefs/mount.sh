#!/bin/bash

echo "Mounting"
sudo mount -t proc /proc /home/heng/nfsroot/ubuntubasefs/proc
sudo mount -t sysfs /sys /home/heng/nfsroot/ubuntubasefs/sys
sudo mount -o bind /dev /home/heng/nfsroot/ubuntubasefs/dev
sudo mount -o bind /dev/pts /home/heng/nfsroot/ubuntubasefs/dev/pts
sudo chroot /home/heng/nfsroot/ubuntubasefs

