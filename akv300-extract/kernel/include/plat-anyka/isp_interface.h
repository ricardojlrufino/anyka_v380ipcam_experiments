#ifndef _ISP_INTERFACE_H
#define _ISP_INTERFACE_H

#define  ISP_PARM_MODE				0x01
#define  ISP_PARM_CHANNEL2			0x02
#define  ISP_PARM_OSD				0x04
#define  ISP_PARM_OCCLUSION			0x08
#define  ISP_PARM_OCCLUSION_COLOR	0x10
#define  ISP_PARM_ZOOM				0x20

enum isp_working_mode {
	ISP_JPEG_MODE,				// JPEG compression frame mode
	ISP_JPEG_VIDEO,				// JPEG video frame mode
	ISP_YUV_OUT,				// YUV single frame mode. is not support minor channel
	ISP_YUV_BYPASS,				// YUV single frame mode and bypass
	ISP_YUV_MERGER_OUT,
	ISP_YUV_BIG,				// YUV single frame and big image mode
	ISP_YUV_VIDEO_OUT,			// YUV video frame mode
	ISP_YUV_VIDEO_BYPASS,		// YUV video frame mode and bypass
	ISP_YUV_VIDEO_MERGER_OUT,
	ISP_RGB_OUT,				// RGB single frame mode
	ISP_RGB_VIDEO_OUT,			// RGB video frame mode
	ISP_RGB_BIG,				// RGB single frame and big image mode
};

enum isp_mode_class {
	ISP_RGB_CLASS,
	ISP_YUV_CLASS,
	ISP_JPEG_CLASS,
};

struct isp_channel2_info {
	int type;
	int width;
	int height;
	int enable;
};

struct isp_mode_info {
	int type;
	enum isp_working_mode mode;
};

struct isp_osd_info {
	int type;
	int channel;
	int color_depth;
	int color_transparency;
	int start_xpos;
	int end_xpos;
	int start_ypos;
	int end_ypos;
	int enable;
	unsigned long phys_addr;
	unsigned long virt_addr;
};

struct isp_occlusion_info {
	int type;
	int channel;
	int number;
	int start_xpos;
	int end_xpos;
	int start_ypos;
	int end_ypos;
	int enable;
};

struct isp_occlusion_color {
	int type;
	int color_type;
	int transparency;
	int y_component;
	int u_component;
	int v_component;
};

struct isp_zoom_info {
	int type;
	int channel;		// value: 1 indicate isp master channel, 2  indicate the second channel
	int cut_xpos;
	int cut_ypos;
	int cut_width;
	int cut_height;
	int out_width;
	int out_height;
};


/* response pc tool control command */
#define  ISP_CID_BLACK_BALANCE		0x60		//black balance
#define  ISP_CID_LENS				0x61		//lens correct
#define  ISP_CID_DEMOSAIC			0x62		//demosaic
#define  ISP_CID_RGB_FILTER			0x63		//RGB filter, noise reduce
#define  ISP_CID_UV_FILTER			0x64		//uv filter
#define  ISP_CID_DEFECT_PIXEL		0x65		//bad color correct
#define  ISP_CID_WHITE_BALANCE		0x66		//white balance
#define  ISP_CID_AUTO_WHITE_BALANCE	0x67		//auto white balance
#define  ISP_CID_COLOR				0x68		//color correct
#define  ISP_CID_GAMMA				0x69		//gamma calculate
#define  ISP_CID_BRIGHTNESS_ENHANCE	0x70		//brightness edge enhancement
#define  ISP_CID_SATURATION			0x72		//saturation
#define  ISP_CID_HISTOGRAM			0x73		//histogram
#define  ISP_CID_SPECIAL_EFFECT		0x74		//special effect
#define  ISP_CID_SET_SENSOR_PARAM	0x75		//set sensor parameter
#define  ISP_CID_GET_SENSOR_PARAM	0x76		//get sensor parameter

//ycx
#define ISP_CID_AE_CTRL_PARAM		0x77		//get AE parameter
#define ISP_CID_CC_AWB_PARAM		0x78		//get AE parameter


/* response pc tool control command structure define */
struct isp_black_balance {
	int type;
	int	enable;
	unsigned int r_offset;		//register table 0x20: offset register 13 [31:22]
	unsigned int g_offset;		//register table 0x20: offset register 14 [31:22]
	unsigned int b_offset;		//register table 0x20: offset register 15 [31:22]
};

struct isp_lens_correct {
	int type;
	int	enable;					//register table 0x20: offset register 24 [31]
	int lens_coefa[10];			//register table 0x20: start offset register 13 [21:0]
	int lens_coefb[10];
	int lens_coefc[10];
	unsigned int lens_range[10];
	unsigned int lens_xref;		//register table 0x20: offset register 23 [31:22]
	unsigned int lens_yref;		//register table 0x20: offset register 23 [31:22]
};

//demosaic
struct isp_demosaic {
	int type;
	int	enable;					//register table 0x20: offset register 2 [12]
	unsigned int threshold;		//register table 0x20: offset register 2 [11:0]
};

// noise reduce
struct isp_rgb_filter {
	int type;
	int	enable;					//register table 0x24: offset register 1 [31]
	unsigned int threshold;		//register table 0x24: offset register 1 [21:12]
};

//uv iso filter
struct isp_uv_filter {
	int type;
	int	enable;					//register table 0x24: offset register 13 [30]
};

// defect pixel 
struct isp_defect_pixel {
	int type;
	int	enable;					//register table 0x24: offset register 1 [30]
	unsigned int threshold;		//register table 0x24: offset register 1 [29:28]
};

struct isp_white_balance {
	int type;
	int	enable;
	unsigned int co_r;		//register table 0x24: offset register 0 [11:0]
	unsigned int co_g;		//register table 0x24: offset register 0 [23:12]
	unsigned int co_b;		//register table 0x24: offset register 1 [11:0]
};

struct isp_auto_white_balance {
	int type;
	int	enable;				//register table 0x20: offset register 25 [31]
	int awb_step;
	int index;
	unsigned int gr_low;              // 0.8125
    unsigned int gr_high;             // 1.555
    unsigned int gb_low;              // 0.863
    unsigned int gb_high;             // 1.820
    unsigned int grb_low;             // 0.461
    unsigned int grb_high;            // 1.559
    //range of pixel 
    unsigned int r_low;
    unsigned int r_high;
    unsigned int g_low;
    unsigned int g_high;
    unsigned int b_low;
    unsigned int b_high;

	unsigned int co_r;		//register table 0x24: offset register 0 [11:0]
	unsigned int co_g;		//register table 0x24: offset register 0 [23:12]
	unsigned int co_b;		//register table 0x24: offset register 1 [11:0]
};

struct isp_color_correct {
	int type;
	int	enable;					//register table 0x24: offset register 11 [31]
	unsigned int cc_thrs_low;
	unsigned int cc_thrs_high;
	int ccMtrx[3][3];
};

struct isp_gamma_calculate {
	int type;
	int	enable;					//register table 0x24: offset register 11 [30]
	int	is_sync;				//count: 0: 1
	unsigned int gamma[32];		//register table 0x28: 64 register
};

//brightness edge enhancement
struct isp_brightness_enhance {
	int type;
	int	enable;				//register table 0x24: offset register 11 [29]
	int	ygain;				//register table 0x24: offset register 12 [31:24]
    unsigned int y_thrs;	//register table 0x24: offset register 12 [22:12]
    unsigned int y_edgek;	//register table 0x24: offset register 12 [10:0]
};

struct isp_saturation {
	int type;
	int	enable;				//register table 0x24: offset register 13 [31]
	int	Khigh;				//register table 0x24: offset register 13 [29:20], (0.1~3) *256 _8_8 
    int	Klow;				//register table 0x24: offset register 13 [19:10], register value  (0.1~3)  
    int	Kslope;				//register table 0x24: offset register 13 [9:0], (ih-il)*256/(ch-cl)
	unsigned int Chigh;		//register table 0x24: offset register 14 [31:24],0~255
	unsigned int Clow;		//register table 0x24: offset register 14 [23:16], 0~255  
};

struct isp_histogram {
	int type;
	int	enable;					//
	int	sync;					//sync: 0: 1
	unsigned int hist[32];		//
};

struct isp_special_effect {
	int type;
	int	enable;					//register table 0x24: offset register 11 [28]
	int	solar_enable;			//register table 0x24: offset register 11 [27]
	unsigned int solar_thrs;	//register table 0x24: offset register 0 [31:24]
	int          y_eff_coefa;	//register table 0x24: offset register 14 [15:8]
    unsigned int y_eff_coefb;	//register table 0x24: offset register 14 [7:0]
	int          u_eff_coefa;  	//register table 0x24: offset register 15 [31:24]
    unsigned int u_eff_coefb;	//register table 0x24: offset register 15 [23:16]
	int          v_eff_coefa;	//register table 0x24: offset register 15 [15:8]
    unsigned int v_eff_coefb;	//register table 0x24: offset register 15 [7:0]
};

//uv iso filter
struct isp_image_fog {
	int type;
	int	enable;					//register table 0x24: offset register 13 [30]
};

struct isp_config_sensor_reg {
	int type;
	int	enable;
	unsigned int cmd;
	unsigned int data;
};


struct isp_ae_attr {
	int type;
	int enable; 			// enable AE

	unsigned int ae_target;
	unsigned int ae_tolerance;
	unsigned int ae_step;
	unsigned int ae_timemax;
	unsigned int ae_timemin;
	
	int enableExpLinkage;
	int enableExpCompensation;
	int enableExpGama;
};


struct isp_color_correct_awb {
	int type;				
	int color_temperture_index;
	unsigned int cc_thrs_low;
	unsigned int cc_thrs_high;
	int ccMtrx[3][3];
};


#endif

