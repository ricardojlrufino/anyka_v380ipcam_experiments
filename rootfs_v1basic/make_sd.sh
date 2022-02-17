#!/bin/bash

SDCARD_SIZE="7,5G"
device=`lsblk | grep $SDCARD_SIZE | awk '{print $1}'`

rm root.sqsh
mksquashfs rootfs root.sqsh
echo "[CAUTION!] DD to /dev/${device}2"
[ -z "${device}" ] && exit 1
sudo dd if=root.sqsh of=/dev/${device}2  status=progress bs=1MB
sync
umount /dev/sdc1
umount /dev/sdc2
