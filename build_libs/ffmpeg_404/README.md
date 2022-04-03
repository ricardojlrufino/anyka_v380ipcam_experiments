 
## Crosscompile

Download: https://ffmpeg.org/releases/ffmpeg-4.0.4.tar.gz


export STAGING_DIR=/media/ricardo/Dados/Outros/Linux/tools/arm-anykav200-crosstool/usr
export PATH="${STAGING_DIR}/bin:$PATH"
export LD_LIBRARY_PATH=$STAGING_DIR/lib:$LD_LIBRARY_PATH
export TARGET=arm-anykav200-linux-uclibcgnueabi
export CROSS=arm-anykav200-linux-uclibcgnueabi
export CROSS_COMPILE=${CROSS}-
export BUILD=x86_64-pc-linux-gnu
export INSTRALL_DIR="/media/ricardo/Dados/Outros/Linux/IPCAM/Camera-ANYKA-v380/anyka_v380ipcam_experiments/build_libs/ffmpeg/"


./configure --enable-cross-compile  --cross-prefix=${CROSS_COMPILE} \
--arch=armv --target-os=linux \
--prefix=${INSTRALL_DIR} --enable-gpl \
--extra-cflags="-O3 -march=armv5te" \
--enable-nonfree \
--enable-shared --disable-static --disable-debug --enable-pthreads  \
--extra-libs=-ldl 