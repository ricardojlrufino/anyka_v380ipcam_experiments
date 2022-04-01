#!/bin/sh
APP=${1:-TestServer}
export LD_LIBRARY_PATH=/lib/java:/lib:/usr/lib:/oldcam/usr/lib
set -x
/mnt/sdcard/CAM/ejre1.7.0_55/bin/java -Xms10m -Xmx10m $APP
