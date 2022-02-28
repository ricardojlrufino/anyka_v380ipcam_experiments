 
# ANYKA SETUP variables
# BY RJLR

export TOOLCHAIN_PATH=/media/ricardo/Dados/Outros/Linux/tools/arm-anykav200-crosstool/usr

echo "Using Toolchain: $TOOLCHAIN_PATH"

export CROSSPATH=$TOOLCHAIN_PATH/bin
export STAGING_DIR=$TOOLCHAIN_PATH
export PATH="$CROSSPATH:$PATH"
export LD_LIBRARY_PATH=$STAGING_DIR/lib:$LD_LIBRARY_PATH

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

## import functions do deploy
DIR=$(dirname "${BASH_SOURCE[0]}")   # Get the directory name
DIR=$(realpath "${DIR}")    # Resolve its full path if need be
source ${DIR}/../scripts/send_ftp.sh

