/* gc0308 Camera
 *
 * Copyright (C) 2008 Renesas Solutions Corp.
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __AK_SENSOR_H__
#define __AK_SENSOR_H__

#include <media/soc_camera.h>
#include <linux/videodev2.h>
#include <plat-anyka/ak_isp_drv.h>

#define SENSOR_MAX_SUPPORT        20

#define V4L2_CID_SENSOR_POWERON		(V4L2_CID_USER_CLASS + 1)
#define V4L2_CID_SENSOR_POWEROFF	(V4L2_CID_USER_CLASS + 2)
#define V4L2_CID_SENSOR_STANDBYIN	(V4L2_CID_USER_CLASS + 3)
#define V4L2_CID_SENSOR_STANDBYOUT	(V4L2_CID_USER_CLASS + 4)
#define V4L2_CID_SENSOR_MCLK		(V4L2_CID_USER_CLASS + 5)
#define V4L2_CID_SENSOR_ID			(V4L2_CID_USER_CLASS + 6)
#define V4L2_CID_SENSOR_FPS			(V4L2_CID_USER_CLASS + 7)
#define V4L2_CID_SENSOR_RESOLUTION			(V4L2_CID_USER_CLASS + 8)
#define V4L2_CID_SENSOR_OFFSET			(V4L2_CID_USER_CLASS + 9)

#define SENSOR_SET_PIN_VAL(reset_pin, pwdn_pin)	\
				( (((reset_pin) & 0xff) << 8) | ((pwdn_pin) & 0xff) )

#define SENSOR_GET_PWDN_PIN(val)	\
				( (val) & 0xff )

#define SENSOR_GET_RESET_PIN(val)	\
				( ((val) >> 8) & 0xff )

typedef enum sensor_mirror {
	SENSOR_MIRROR_NORMAL = 0,
	SENSOR_MIRROR_V,
	SENSOR_MIRROR_H,
	SENSOR_MIRROR_FLIP,
} T_SENSOR_MIRROR;

/*
 * aksensor camera info
 */
struct aksensor_camera_info {
	unsigned int	buswidth;
	unsigned int 	flags;
	unsigned int	pin_pwdn;
	unsigned int	pin_reset;
	struct soc_camera_link link;
//	struct aksensor_edge_ctrl edgectrl;
};

struct aksensor_color_format {
	enum v4l2_mbus_pixelcode code;
	enum v4l2_colorspace colorspace;
};

struct aksensor_win_size {
	char                     *name;
	__s32                     left;
	__s32                     top;
	__s32                     width;
	__s32                     height;
};

struct sensor_info {
    const char *sensor_name;	
    int sensor_id;
    const struct v4l2_ctrl_config *ctrls;
    int nr_ctrls;
    const struct aksensor_color_format *formats;
    int num_formats;
	const struct aksensor_win_size *resolution;
	int num_resolution;
    AK_ISP_SENSOR_CB *sensor_callback;
};

typedef struct sensor_i2c_data_s {
	u8  u8DevAddr;
	u32 u32RegAddr;
	u32 u32RegAddrByteNum;
	u32 u32Data;
	u32 u32DataByteNum;
	u32 reserved[2];
} T_SENSOR_I2C_DATA_S;

/** 
 * @brief register a camera device, which will be probed at camera open
 * @author dengzhou/wudaochao
 * @date 2012-11-06
 * @param[in] struct sensor_info
 * @param[in] handler camera interface
 * @retval 0 register successfully
 * @retval -1 register unsuccessfully
 */
int register_sensor(struct sensor_info *si);

int aksensor_module_init(void);
void aksensor_module_exit(void);
AK_ISP_SENSOR_CB *ak_sensor_get_sensor_cb(void);

#endif /* END __AKSENSOR_H__ */

