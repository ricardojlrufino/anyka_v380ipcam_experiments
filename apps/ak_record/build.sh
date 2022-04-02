
# Read enviroment
export TOOLCHAIN_PATH=/media/ricardo/Dados/Outros/Linux/tools/arm-anykav200-crosstool/usr

export PLAT_WORKDIR=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/anyka_v380ipcam_experiments/akv300-extract
#export LIBQUIRC_PATH=/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/TEST_SRCS/quirc/lib
#export LIBALSA_PATH=/media/ricardo/Dados/Outros/Linux/IPCAM/YI_HOME/TESTES/alsa-lib-1.2.4/install/include/

TARGET_DEVICE=192.168.15.64
TARGET_PATH=/mnt/sdcard/CAM/scripts
export TARGET_BIN=ak_record


echo "Using Toolchain: $TOOLCHAIN_PATH"

export CROSSPATH=$TOOLCHAIN_PATH/bin
export STAGING_DIR=$TOOLCHAIN_PATH
export PATH="$CROSSPATH:$PATH"
export LD_LIBRARY_PATH=$STAGING_DIR/lib_v380:$STAGING_DIR/lib:$LD_LIBRARY_PATH

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

make 

file $TARGET_BIN

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

