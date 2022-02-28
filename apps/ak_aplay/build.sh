#!/bin/bash 

export TARGET_BIN=ak_aplay

# set SDK
source /media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/anyka_v380ipcam_experiments/toolchain/activate.sh

export PLAT_WORKDIR=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/anyka_v380ipcam_experiments/akv300-extract


TARGET_DEVICE=192.168.15.64
TARGET_PATH=/mnt/sdcard/CAM/scripts

make clean

make || exit $?

if [ -f "$TARGET_BIN" ]; then
    echo "Uploading to $TARGET_DEVICE:$TARGET_PATH"
 
	send_ftp ${TARGET_DEVICE} ${TARGET_BIN} ${TARGET_PATH}

    echo "-- DONE --"
else
    echo "-- ERROR --"
fi

