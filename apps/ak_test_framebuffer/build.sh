#!/bin/bash 

# Read enviroment
# set SDK
source ../../toolchain/activate.sh

export PLAT_WORKDIR=../../anyka_v380ipcam_experiments/akv300-extract
export LIBDISPLAY_PATH=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/TEST_SRCS/ssd1306
#export LIBALSA_PATH=/media/ricardo/Dados/Outros/Linux/IPCAM/YI_HOME/TESTES/alsa-lib-1.2.4/install/include/

TARGET_DEVICE=192.168.15.64
TARGET_PATH=/mnt/sdcard/CAM/scripts
export TARGET_BIN=ak_test_framebuffer


make clean

make || exit $?

if [ -f "$TARGET_BIN" ]; then
    echo "Uploading to $TARGET_DEVICE:$TARGET_PATH"
 
	send_ftp ${TARGET_DEVICE} ${TARGET_BIN} ${TARGET_PATH}

    echo "-- DONE --"
else
    echo "-- ERROR --"
fi


