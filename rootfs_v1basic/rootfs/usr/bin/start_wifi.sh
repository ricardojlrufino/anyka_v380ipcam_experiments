#!/bin/sh

SDCARD="/mnt/sdcard"
[ ! -d "$SDCARD" ] && SDCARD="/tmp/sdcard"

insmod /usr/modules/atbm603x_wifi_HT20_usb.ko
sleep 2
ifconfig wlan0 mtu 1400
sleep 1
wpa_supplicant -B -iwlan0 -Dwext -c ${SDCARD}/wpa_supplicant.conf
sleep 5
udhcpc -i wlan0
sleep 2

