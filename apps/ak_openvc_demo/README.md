QRCode reading application for IP cameras with ALLWINNER processor and SP2305 MIPI-CSI Sensor

Demonstration: https://youtu.be/o10Hf9vFvKs	

**Stage**: BETA

![](https://github.com/fritz-smh/yi-hack/raw/master/yi.jpg?raw=true)

## 

## Tested cameras

Currently this project tested only in the following cameras:

| Camera | Remarks |
| --- | --- |
| **Yi 1080p Home Y20GA** | y20ga |

## Features

- Detect the qrcode on mobile screens
- Detect the qrcode on paper
- Good capture angle
- Good capture range ([Needs manual changes in the lens focus](https://github.com/ricardojlrufino/yicam_qrcode/wiki/Improve-Focus))
- Readings at night using Night Vision (needs more tests)
- Change Led on Detect 
- PlaySound on Detect (need aplay) [#1](/../../issues/1)
- Write detected qrcode to FIFO file ([see example](test/src/test_read_fifo.c))

### TODO

- [ ] Fix audio setup [#1](/../../issues/1)
- [ ] Create an example that reads the QRCode and send an HTTP request
- [ ] Create the script to restart the application if it fails
- [ ] Create demonstration video

## Performance

The detection of QR Code is done in about 15ms

## Benchmark / Memory

- Preliminary tests point to a 45% use of CPU.
- The system was executed for 10 consecutive hours without presenting outofmemory



## Running

You can use the pre-compilated version !

Before run, you need copy libraries in /libs to SD, and set PATH

> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/sd/my/lib
>
> ./yicam_qrcode



Enable sounds 

>  yicam_qrcode -s

Configure settings

>  See file src/app_settings.h

You can enable snapshots in case of QRCODE recognition failures

>  #define SNAPSHOT_ON_ERROR 1



## Integration

To integrate other applications you can read the data created in a special FIFO file:

> /tmp/qrcode_read

 ([see example](test/src/test_read_fifo.c))

## Compiling

TODO / draft

Use toolchain: 

https://github.com/sipeed/libmaix/releases/download/v0.1.0/toolchain-sunxi-musl-pack-2021-01-09.tar.xz

Edit paths in: `env` file

Run: `./build.sh`



## DISCLAIMER

**NOBODY BUT YOU IS RESPONSIBLE FOR ANY USE OR DAMAGE THIS SOFTWARE MAY CAUSE. THIS IS INTENDED FOR EDUCATIONAL PURPOSES ONLY. USE AT YOUR OWN RISK.**

## Companion Projects

https://github.com/roleoroleo/yi-hack-Allwinner (Custom firmware for Yi 1080p)
