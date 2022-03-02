#!/bin/bash

# Set ARM toolchain path to setup make variables
source ../../toolchain/activate.sh


if [ ! -d node_src ]; then
    echo "Downloading Node for ARMv5"
    git clone --depth 1 https://github.com/nodejs/node -b v0.10.48 node_src
fi

cd node_src

./configure --without-snapshot --prefix=$(pwd)/install
make -j4
make install

mv node_src/install node_install


# clean_node()
# {
#     echo Cleaning node...
#     cd $DEV_ROOT/src/node
#     $MAKE distclean
#     $RM deps/v8/tools/jsmin.pyc
#     $RM tools/gyp/pylib/gyp/__init__.pyc
#     $RM tools/gyp/pylib/gyp/common.pyc
#     $RM tools/gyp/pylib/gyp/generator/__init__.pyc
#     $RM tools/gyp/pylib/gyp/generator/make.pyc
#     $RM tools/gyp/pylib/gyp/input.pyc
#     $RM tools/gyp/pylib/gyp/xcode_emulation.pyc
# }