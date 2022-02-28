/*
 * ak_camera.h - AK camera driver header file
 *
 * Copyright (c) 2008, Paulius Zaleckas <paulius.zaleckas@teltonika.lt>
 * Copyright (C) 2009, Darius Augulis <augulis.darius@gmail.com>
 *
 * Based on PXA camera.h file:
 * Copyright (C) 2003, Intel Corporation
 * Copyright (C) 2008, Guennadi Liakhovetski <kernel@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARCH_CAMERA_H_
#define __ASM_ARCH_CAMERA_H_

#include <mach/map.h>
#include <mach/gpio.h>
#include <plat-anyka/ak_isp_drv.h>

#define AK_CAM_DRV_NAME 		"ak_camera"
#define MAX_VIDEO_MEM			16

#if 0
#define CSI_BUS_FLAGS	(SOCAM_MASTER | SOCAM_HSYNC_ACTIVE_HIGH | \
			SOCAM_VSYNC_ACTIVE_HIGH | SOCAM_VSYNC_ACTIVE_LOW | \
			SOCAM_PCLK_SAMPLE_RISING | SOCAM_PCLK_SAMPLE_FALLING | \
			SOCAM_DATA_ACTIVE_HIGH | SOCAM_DATA_ACTIVE_LOW | \
			SOCAM_DATAWIDTH_8)
#endif
			
#define CSI_BUS_FLAGS	(V4L2_MBUS_MASTER | V4L2_MBUS_HSYNC_ACTIVE_HIGH | \
				V4L2_MBUS_VSYNC_ACTIVE_HIGH | V4L2_MBUS_VSYNC_ACTIVE_LOW | \
				V4L2_MBUS_PCLK_SAMPLE_RISING | V4L2_MBUS_PCLK_SAMPLE_FALLING | \
				V4L2_MBUS_DATA_ACTIVE_HIGH | V4L2_MBUS_DATA_ACTIVE_LOW)

#define AK_CAMERA_MASTER			1
#define AK_CAMERA_DATAWIDTH_4		2
#define AK_CAMERA_DATAWIDTH_5		4
#define AK_CAMERA_DATAWIDTH_8		8
#define AK_CAMERA_DATAWIDTH_9		0x10
#define AK_CAMERA_DATAWIDTH_10		0x20

#define AK_CAMERA_VSYNC_HIGH		0x100
#define AK_CAMERA_PCLK_RISING		0x200
#define AK_CAMERA_DATA_HIGH			0x400

/* Image Sensor Command Register */
#define CICR_DATA_FMT	(0x3 << 0)
#define CICR_HSCAL_EN	(1 << 2)
#define CICR_VSCAL_EN	(1 << 3)
#define CICR_MODE		(1 << 4)
#define CICR_FULL_RANGE_YUV		(1 << 5)
#define CICR_DATA_FMT_VAL(x)	(((x) << 0) & CICR_DATA_FMT)


/* Personal */


/** @{@name IMAGE sensor module register and bit map define
 */
/* image capturing command */
#define IMG_CMD                                0x0000
/* Source/Destination image horizontal length */
#define IMG_HINFO1                           0x0004
/* Horizontal scalling information */
#define IMG_HINFO2					0x0008
/* Source/Destination image vertical length */
#define IMG_VINFO1                           0x000C
/* Horizontal scalling information */
#define IMG_VINFO2                         	0x0010

/* DMA starting address of external RAM for Y component of odd frame */                                                        
#define IMG_YODD                          	0x0018
#define IMG_UODD                          	0x001c
#define IMG_VODD                           	0x0020
#define IMG_RGBODD                       	0x0024
#define IMG_YEVE                            	0x0028
#define IMG_UEVE                            	0x002c
#define IMG_VEVE                            	0x0030
#define IMG_RGBEVE                        	0x0034
/* Image sensor configuration */
#define IMG_CONFIG                        	0x0040
/* Status of the current frame */
#define IMG_STATUS                        	0x0060
/* The line number of a frame when JPEG-compressed format */
#define IMG_NUM                             	0x0080
/** @} */


/** @{@name IMAGE sensor module register and bit map define
 */
/* Multiple function control register */ 
#define MUL_FUN_CTL_REG                                 (AK_VA_SYSCTRL + 0x0058)
/* Multiple function control register2 */ 
#define MUL_FUN_CTL_REG2								(AK_VA_SYSCTRL + 0x0014)
/** @} */
#define MUL_RESET_CTRL_REG								(AK_VA_SYSCTRL + 0x0010)

/* camera private cid & data defined*/
/***********************************/
enum camera_pcid {
	PCID_CH2_OUTPUT_FMT		= 1,
	PCID_A_FRAME_RAW,
};

/* move to ispdrv_interface.h
enum camera_interface {
	DVP_INTERFACE = 0,
	MIPI_INTERFACE
};*/

struct pcid_ch2_output_fmt_data {
	int type;
	int width;
	int height;
};
/***********************************/

struct captureSync{
	unsigned long long adcCapture_bytes;
	struct timeval tv;
	unsigned int rate;
	unsigned int frame_bits;
};


/**
 * struct ak_camera_pdata - AK camera platform data
 * @mclk:	master clock frequency in MHz units
 * @flags:	AK camera platform flags
 */
struct ak_camera_pdata {
	unsigned long mclk;
	unsigned long flags;
	//enum camera_interface interface;    //setup interface in sensor drivers
};

#endif /* __ASM_ARCH_CAMERA_H_ */

