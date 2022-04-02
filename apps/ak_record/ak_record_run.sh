#!/bin/sh

insmod /usr/modules/sensor_sc1245.ko
insmod /usr/modules/akcamera.ko
insmod /usr/modules/ak_info_dump.ko

export LD_LIBRARY_PATH=/oldcam/lib:/oldcam/usr/lib
rm -rf /mnt/sdcard/CAM/record
mkdir /mnt/sdcard/CAM/record

./ak_record