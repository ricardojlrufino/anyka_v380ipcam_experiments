
# Read enviroment
export TOOLCHAIN_PATH=/media/ricardo/Dados/Outros/Linux/tools/arm-anykav200-crosstool/usr

export PLAT_WORKDIR=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/BUILDS/akv300-extract
export OPENCV_PATH=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/TEST_SRCS/opencv-2.4.13/build_jpeg/_install
export CAMLIBS_PATH=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/anyka_v380ipcam_experiments/cam_orig_libs

export LD_LIBRARY_PATH=${TOOLCHAIN_PATH}/lib:${CAMLIBS_PATH}/mvs/lib:{PLAT_WORKDIR}/libplat/lib_v380:{PLAT_WORKDIR}/libplat/lib:$STAGING_DIR/lib:$LD_LIBRARY_PATH

TARGET_DEVICE=192.168.15.64
TARGET_PATH=/mnt/sdcard/CAM/scripts
export TARGET_BIN=ak_openvc_demo


echo "Using Toolchain: $TOOLCHAIN_PATH"

export CROSSPATH=$TOOLCHAIN_PATH/bin
export STAGING_DIR=$TOOLCHAIN_PATH
export PATH="$CROSSPATH:$PATH"


export TARGET=arm-anykav200-linux-uclibcgnueabi
export CROSS=arm-anykav200-linux-uclibcgnueabi
export BUILD=x86_64-pc-linux-gnu
export CROSSPREFIX=${CROSS}-
export STRIP=${CROSSPREFIX}strip
export CXX=${CROSSPREFIX}g++
export CC=${CROSSPREFIX}gcc
export LD=${CROSSPREFIX}ld
export AS=${CROSSPREFIX}as
export AR=${CROSSPREFIX}ar
export RANLIB=${CROSSPREFIX}ranlib

make clean

make || exit $?

if [ -f "$TARGET_BIN" ]; then
    echo "Uploading to $TARGET_DEVICE:$TARGET_PATH"
    # scp $BIN $TARGET_DEVICE:$TARGET_PATH

ftp ${TARGET_DEVICE} <<EOF 
pwd
cd ${TARGET_PATH}
ls
put ${TARGET_BIN}
chmod +x ${TARGET_BIN}
ls
EOF
    echo "-- DONE --"
else
    echo "-- ERROR --"
fi

