#!/bin/bash

# Set ARM toolchain path to setup make variables
source ../../toolchain/activate.sh


# use old v0.10.x node 
export PATH="../v0.10_linux64/bin:$PATH";


echo "Node  Version $(node -v)"
echo "NPM Version $(npm -v)"

if [[ ! -d node_modules ]]; then
npm --target_arch=arm-linux install
npm --target_arch=arm-linux run configure
fi

npm --target_arch=arm-linux run build
npm pack


# add Binaries and ZIP
LIBNAME=cpp-demo-0.0.1
tar zxvf ${LIBNAME}.tgz
cp -R build/Release package
(cd package && zip -r ../${LIBNAME}.zip  .)
rm -rf ${LIBNAME}.tgz package
mv ${LIBNAME}.zip build/



# send to device
cd build
TARGET_BIN=${LIBNAME}.zip
TARGET_DEVICE=192.168.15.64
TARGET_PATH=/mnt/sdcard/CAM/access_control/test-capi
send_ftp ${TARGET_DEVICE} ${TARGET_BIN} ${TARGET_PATH}
