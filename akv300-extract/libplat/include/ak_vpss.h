#ifndef __AK_VPSS_H__
#define __AK_VPSS_H__

/********************** osd *******************************/
#define VPSS_OSD_CHANNELS_MAX 2
#define VPSS_OSD_NUM_PER_VIDEO_CHN 3

/********************** md *******************************/
#define VPSS_MD_DIMENSION_H_MAX 32
#define VPSS_MD_DIMENSION_V_MAX 24

/********************** mask *******************************/
#define VPSS_MASK_AREA_MAX	4

/********************** od *******************************/
#define VPSS_OD_AF_STATICS_MAX	5
#define VPSS_OD_RGB_HIST_MAX	256

/********************** auto ir switch *******************************/
#define NIGHT_ARRAY_NUM		5
#define DAY_ARRAY_NUM		10

// auto day night switch threshold
#define DAY_TO_NIGHT_LUM_FACTOR		(3072)
#define NIGHT_TO_DAY_LUM_FACTOR		(2048)

#define TOTAL_CNT_NIGHT_0			(1200)
#define TOTAL_CNT_NIGHT_1			(1200)
#define TOTAL_CNT_NIGHT_2			(1200)
#define TOTAL_CNT_NIGHT_3			(1200)
#define TOTAL_CNT_NIGHT_4			(1200)

#define TOTAL_CNT_DAY_0				(50000)
#define TOTAL_CNT_DAY_1				(50000)
#define TOTAL_CNT_DAY_2				(50000)
#define TOTAL_CNT_DAY_3				(50000)
#define TOTAL_CNT_DAY_4				(50000)
#define TOTAL_CNT_DAY_5				(50000)
#define TOTAL_CNT_DAY_6				(50000)
#define TOTAL_CNT_DAY_7				(50000)
#define TOTAL_CNT_DAY_8				(50000)
#define TOTAL_CNT_DAY_9				(50000)

#define LOCK_TIME					(1800000)



/********************** osd *******************************/
enum vpss_osd_set_id {
	OSD_SET_COLOR_TABLE = 1,	
	OSD_SET_MAIN_CHANNEL_DATA,
	OSD_SET_SUB_CHANNEL_DATA,	
	OSD_SET_MAIN_DMA_MEM_REQUST,
	OSD_SET_SUB_DMA_MEM_REQUST,
};

struct vpss_osd_param {
	enum vpss_osd_set_id id;
	unsigned char data[128];
};

/********************** md *******************************/
struct vpss_md_info {
	unsigned short stat_max;
	unsigned short stat[VPSS_MD_DIMENSION_V_MAX][VPSS_MD_DIMENSION_H_MAX];
	unsigned int level;
};

/********************** mask *******************************/
enum vpss_mask_color_type {
	VPSS_MASK_ORIGINAL_VIDEO = 0,	/* masked with original video data*/
	VPSS_MASK_MOSAIC_VIDEO,			/* masked with mosaic video data*/
	VPSS_MASK_MOSAIC_WHITE_VIDEO,	/* masked with mosaic & white 50% video data*/
	VPSS_MASK_MOSAIC_BLACK_VIDEO,	/* masked with mosaic & black 50% video data*/
};

struct vpss_mask_color_info {
	/* defined by enum vpss_mask_color_type*/
	unsigned char color_type;

	/* range [0, 0xff]. if 0 mask video opaquely, then can not see image */
	unsigned char mk_alpha;

	/* range [0, 0xff] */
	unsigned char y_mk_color;
	/* range [0, 0xff] */
	unsigned char u_mk_color;
	/* range [0, 0xff] */
	unsigned char v_mk_color;
};

struct vpss_mask_area {
	unsigned short start_xpos;
	unsigned short end_xpos;
	unsigned short start_ypos;
	unsigned short end_ypos;
	unsigned char enable;
};

struct vpss_mask_area_info {
	struct vpss_mask_area main_mask[VPSS_MASK_AREA_MAX];
	struct vpss_mask_area sub_mask[VPSS_MASK_AREA_MAX];
};

/********************** od *******************************/
struct vpss_od_info {
	unsigned int af_statics[VPSS_OD_AF_STATICS_MAX];
	unsigned int rgb_total;
	unsigned int rgb_hist[VPSS_OD_RGB_HIST_MAX];
};

struct vpss_af_stat_info {
   unsigned int  af_statics[5];
};

struct vpss_af_attr {
    unsigned short  af_win0_left; 	//[0, 1279] 
	unsigned short  af_win0_right;	//[0, 1279]
	unsigned short  af_win0_top;	    //[0, 959]
	unsigned short  af_win0_bottom;   //[0, 959]

	unsigned short  af_win1_left; 	//[0, 1279]
	unsigned short  af_win1_right;	//[0, 1279]
	unsigned short  af_win1_top;	    //[[0, 959]
	unsigned short  af_win1_bottom;   //[0, 959]

	unsigned short  af_win2_left; 	//[0, 1279]
	unsigned short  af_win2_right;	//[0, 1279]
	unsigned short  af_win2_top;	    //[0, 959]
	unsigned short  af_win2_bottom;   //[0, 959]

    unsigned short  af_win3_left; 	//[0, 1279]
	unsigned short  af_win3_right;	//[0, 1279]
	unsigned short  af_win3_top;	    //[0, 959]
	unsigned short  af_win3_bottom;   //[0, 959]

    unsigned short  af_win4_left; 	//[0, 1279]
	unsigned short  af_win4_right;	//[0, 1279]
	unsigned short  af_win4_top;	    //[0, 959]
	unsigned short  af_win4_bottom;   //[0, 959]
    
	unsigned short   af_th;       //[0, 128],     af_thΪ��ֵ�����ڸ�ֵ�ĸ�ͨ�˲����������ȡ�����и�ͨ�˲����������sharpģ���еĸ�Ƶ�񻯽����
    
};



/********************** effect *******************************/
enum vpss_effect_type {
	/* HUE to SHARP, value: [-50, 50], 0 means use the value in ISP config file */
	VPSS_EFFECT_HUE = 0x00,
	VPSS_EFFECT_BRIGHTNESS,
	VPSS_EFFECT_SATURATION,
	VPSS_EFFECT_CONTRAST,
	VPSS_EFFECT_SHARP,
	VPSS_EFFECT_WDR,
	
	VPSS_STYLE_ID,		//[0, 2]
	VPSS_POWER_HZ		//50 or 60
};

/********************** ISP *******************************/
struct vpss_isp_ae_attr {
	unsigned long exp_time_max;			//�ع�ʱ������ֵ
    unsigned long exp_time_min;			//�ع�ʱ�����Сֵ
    unsigned long d_gain_max;      		//������������ֵ
    unsigned long d_gain_min;     		//�����������Сֵ
    unsigned long isp_d_gain_min;  		//isp�����������С
    unsigned long isp_d_gain_max;  		//isp������������ֵ
    unsigned long a_gain_max;     		//ģ����������ֵ
    unsigned long a_gain_min;      		//ģ���������Сֵ
    unsigned long exp_step;            	//�û��ع��������
    unsigned long exp_stable_range;     //�ȶ���Χ
    unsigned long target_lumiance;     	//Ŀ������
    unsigned long envi_gain_range[10][2];
    unsigned long hist_weight[16];
    unsigned long OE_suppress_en;
    unsigned long OE_detect_scope;		//[0,255]
    unsigned long OE_rate_max;			//[0, 255]
    unsigned long OE_rate_min;			//[0, 255]
};

struct vpss_isp_mae_attr {
	unsigned long exp_time;	
    unsigned long a_gain; 
    unsigned long d_gain;	
    unsigned long isp_d_gain;	
};

struct vpss_isp_wb_type_attr {
    unsigned short wb_type;
};

struct vpss_isp_3d_nr {
	unsigned short uv_min_enable;
	unsigned short tnr_y_enable;
	unsigned short tnr_uv_enable;
	unsigned short updata_ref_y;		//����Y
	unsigned short updata_ref_uv;		//����uv
	unsigned short tnr_refFrame_format;	//�ο�֡�ĸ�ʽ
	unsigned short y_2dnr_enable;
	unsigned short uv_2dnr_enable;

	unsigned short uvnr_k; 				//[0, 15]
	unsigned short uvlp_k; 				//[0, 15]
	unsigned short t_uv_k; 				//[0, 127]
	unsigned short t_uv_minstep;		//[0,31]
	unsigned short t_uv_mf_th1;			//[0, 8191]
	unsigned short t_uv_mf_th2;			//[0, 8191]
	unsigned short t_uv_diffth_k1; 		//[0, 255]
	unsigned short t_uv_diffth_k2; 		//[0, 255]
	unsigned short t_uv_diffth_slop;	//[0, 255]
	unsigned short t_uv_mc_k;			//[0-31]
	unsigned short t_uv_ac_th; 			//[0, 1023]

	unsigned short ynr_weight_tbl[17];
	unsigned short ynr_calc_k;
	unsigned short ynr_k;				//[0, 15]
	unsigned short ynr_diff_shift; 		//[0,1]
	unsigned short ylp_k;				//[0, 15]
	unsigned short t_y_th1;				//[0, 255]
	unsigned short t_y_k1; 				//[0, 127]
	unsigned short t_y_k2; 				//[0, 127]
	unsigned short t_y_kslop;			//[0, 127]
	unsigned short t_y_minstep;			//[0-31]
	unsigned short t_y_mf_th1; 			//[0, 8191]
	unsigned short t_y_mf_th2; 			//[0, 8191]
	unsigned short t_y_diffth_k1;		//[0, 255]
	unsigned short t_y_diffth_k2;		//[0, 255]
	unsigned short t_y_diffth_slop;		//[0, 255]
	unsigned short t_y_mc_k;			//[0-31]
	unsigned short t_y_ac_th;	    	//[0, 1023]

	unsigned long md_th;				//[0, 65535]  �˶������ֵ [0-127]
};

struct vpss_isp_3d_nr_attr {
    unsigned short        _3d_nr_mode;
    struct vpss_isp_3d_nr manual_3d_nr;
    struct vpss_isp_3d_nr linkage_3d_nr[9];
};

struct vpss_isp_mwb_attr {
    unsigned short r_gain;
    unsigned short g_gain;
    unsigned short b_gain;
    short r_offset;
    short g_offset;
    short b_offset;
};

struct vpss_isp_awb_attr {
    unsigned short g_weight[16];
    unsigned short y_low;				//y_low<=y_high
    unsigned short y_high;
	unsigned short err_est;
    unsigned short gr_low[10];			//gr_low[i]<=gr_high[i]
    unsigned short gb_low[10]; 			//gb_low[i]<=gb_high[i]
    unsigned short gr_high[10];
    unsigned short gb_high[10];
    unsigned short rb_low[10];			//rb_low[i]<=rb_high[i]
    unsigned short rb_high[10];

    //awb���������Ҫ���õĲ���
    unsigned short auto_wb_step;                //��ƽ�ⲽ������
    unsigned short total_cnt_thresh;            //���ظ�����ֵ
    unsigned short colortemp_stable_cnt_thresh; //�ȶ�֡��������֡һ����Ϊ����ɫ�¸ı�
    unsigned short colortemp_envi[10];
};

struct vpss_isp_exp_type {
    unsigned short  exp_type;
};

struct vpss_isp_ae_run_info {
    unsigned char current_calc_avg_lumi;         		//���ڵļ����������ֵ
    unsigned char current_calc_avg_compensation_lumi;	//�����عⲹ���������ֵ

    unsigned char current_darked_flag;                 	//�����ҹ�ı��

    long  current_a_gain;				//ģ�������ֵ
    long  current_d_gain;				//���������ֵ
    long  current_isp_d_gain;			//isp���������ֵ
    long  current_exp_time;				//�ع�ʱ���ֵ

    unsigned long  current_a_gain_step;	//���ڵ�ģ������Ĳ���
    unsigned long  current_d_gain_step; //��������Ĳ���
    unsigned long  current_isp_d_gain_step;	//isp��������Ĳ���
    unsigned long  current_exp_time_step;	//�ع�ʱ��Ĳ���
};

struct vpss_isp_awb_stat_info {
	//�ڰ�ƽ��ͳ�Ʋ�����Χ�ڵİ�ƽ��ͳ�ƽ��
    unsigned long  total_R[10];	//10��ɫ��ÿһ��ɫ���µ�R��������ֵ
    unsigned long  total_G[10]; //10��ɫ��ÿһ��ɫ���µ�G��������ֵ
    unsigned long  total_B[10]; //10��ɫ��ÿһ��ɫ���µ�B��������ֵ
    unsigned long  total_cnt[10];	//�ڰ�ƽ��ͳ�Ʋ�����Χ�ڵ���������ֵ
    
    //�����Զ���ƽ���㷨����İ�ƽ������ֵ
    unsigned short  r_gain;	// ��ɫ��������ֵ
    unsigned short  g_gain;	// ��ɫ��������ֵ
    unsigned short  b_gain;	// ��ɫ��������ֵ
    unsigned short  r_offset;	// ��ɫ����ƫ��ֵ
    unsigned short  g_offset;	// ��ɫ����ƫ��ֵ
    unsigned short  b_offset;	// ��ɫ����ƫ��ֵ
    unsigned short  current_colortemp_index;   //����ɫ�±�ǣ��ǲ����滷���仯��ɫ��ָ��  
    unsigned short  colortemp_stable_cnt[10];  //ÿһ��ɫ���ȶ���֡������   
};

struct vpss_isp_weight_attr {
   unsigned short zone_weight[8][16];
};

struct vpss_isp_sensor_reg_info {
    unsigned short reg_addr;
    unsigned short value;
};

/* notice: not suggest to use */
struct ak_ir_auto_check {
	int pre_ir_level;		// pre ir cut level,1:day ,0:night
	int day_to_night_lum;	// day to night lum value
	int night_to_day_lum;	// night to day lum value
	int night_cnt[NIGHT_ARRAY_NUM];	// awb night cnt array
	int day_cnt[DAY_ARRAY_NUM];	// awb day cnt array
	int lock_time;					// locke night status time
	int quick_switch_mode;			// quick switch mode
};

struct ak_auto_day_night_threshold {
	int day_to_night_lum;	// day to night lum value
	int night_to_day_lum;	// night to day lum value
	int night_cnt[NIGHT_ARRAY_NUM];	// awb night cnt array
	int day_cnt[DAY_ARRAY_NUM];	// awb day cnt array
	int lock_time;					// locke night status time
	int quick_switch_mode;			// quick switch mode
};

enum mode_state {
	STATE_NIGHT,
	STATE_DAY
};

struct ak_manual_blc {
    short     bl_r_offset;			//[-2048,2047]
    short     bl_gr_offset;			//[-2048,2047]
    short     bl_gb_offset;			//[-2048,2047]
    short     bl_b_offset;			//[-2048,2047]
};

struct vpss_isp_wdr {
    unsigned short   hdr_uv_adjust_level;        //uv调整程度, [0,31]
    unsigned short   hdr_cnoise_suppress_slop;   //抑制斜率
    unsigned short   wdr_enable;
  
	unsigned short	wdr_th1;	  //0-1023
	unsigned short	wdr_th2;	  //0-1023
	unsigned short	wdr_th3;	  //0-1023
	unsigned short	wdr_th4;	  //0-1023
	unsigned short	wdr_th5;	  //0-1023

    //unsigned short wdr_light_weight;
    
	unsigned short	area_tb1[65];	  //10bit
	unsigned short	area_tb2[65];	  //10bit
	unsigned short	area_tb3[65];	  //10bit
	unsigned short	area_tb4[65];	  //10bit
	unsigned short	area_tb5[65];	  //10bit
	unsigned short	area_tb6[65];	  //10bit

	unsigned short	area1_key[16];
	unsigned short	area2_key[16];
	unsigned short	area3_key[16];
	unsigned short	area4_key[16];
	unsigned short	area5_key[16];
	unsigned short	area6_key[16];

    unsigned short   hdr_uv_adjust_enable;       //uv调整使能
    unsigned short   hdr_cnoise_suppress_yth1;   //色彩噪声亮度阈�?
    unsigned short   hdr_cnoise_suppress_yth2;   //色彩噪声亮度阈�?
    unsigned short   hdr_cnoise_suppress_gain;   //色差抑制
};

struct vpss_wdr_attr {
	unsigned short      wdr_mode;              //模式选择，手动或者联�?	struct vpss_isp_wdr manual_wdr;
	struct vpss_isp_wdr linkage_wdr[9]; 
};


/********************** public *******************************/

/**
 * ak_vpss_get_version - get vpss version
 * return: version string
 */
const char *ak_vpss_get_version(void);

/**
 * ak_vpss_init - vpss module init
 * @vi_handle[IN]: opened vi handle
 * @dev_no[IN]: dev number
 * return: 
 * notes: 
 */
void ak_vpss_init(void *vi_handle, int dev_no);


/**
 * ak_vpss_destroy - vpss module destroy
 * @dev_no[IN]: dev number
 * return: 
 * notes: 
 */
void ak_vpss_destroy(int dev_no);


/********************** osd *******************************/
/**  
 * ak_vpss_osd_set_param : set osd param to isp and then to driver
 * @vi_handle[IN]: vi module handle
 * @param[IN]: the osd param  
 * return: 0 success, otherwise failed
 * notes: 
 */
int ak_vpss_osd_set_param(const void *vi_handle, struct vpss_osd_param *param);

/**  
 * ak_vpss_osd_get_param: get osd param to isp and then to driver
 * @vi_handle[IN]: vi module handle
 * @param[OUT]: the osd param  
 * return: 0 success, otherwise failed
 * notes: 
 */
int ak_vpss_osd_get_param(const void *vi_handle, struct vpss_osd_param *param);

/**  
 * ak_vpss_osd_close: close specifically osd channel
 * @vi_handle[IN]: vi module handle
 * @param[IN]: the osd param  
 * return: 0 success, otherwise failed
 * notes: 
 */
int ak_vpss_osd_close(const void *vi_handle, struct vpss_osd_param *param);

/********************** md *******************************/
/**
 * ak_vpss_md_get_stat: get motion detection stat params
 * @vi_handle[IN]: vi module handle
 * @md[OUT]: md params 
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_md_get_stat(const void *vi_handle, struct vpss_md_info *md);

/********************** af stat *******************************/

/**
 * ak_vpss_af_get_stat: get af stat info
 * @vi_handle[IN]: vi module handle
 * @af_stat[OUT]: af_stat info
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_af_get_stat(const void *vi_handle, struct vpss_af_stat_info *af_stat);

/**
 * ak_vpss_set_af_attr: set af attr
 * @vi_handle[IN]: vi module handle
 * @af_stat[IN]: af_attr
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_set_af_attr(const void *vi_handle, struct vpss_af_attr *af_attr);

/**
 * ak_vpss_get_af_attr: set af attr
 * @vi_handle[IN]: vi module handle
 * @af_stat[OUT]: af_attr
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_get_af_attr(const void *vi_handle, struct vpss_af_attr *af_attr);


/********************** mask *******************************/
/**  
 * brief: set main & sub channel mask area
 * @vi_handle[IN]: vi module handle
 * @p_mask[IN]: main & sub channel mask area paramters
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_vpss_mask_set_area(const void *vi_handle, 
		const struct vpss_mask_area_info *mask);

/**  
 * brief: get main & sub channel mask area
 * @vi_handle[IN]: vi module handle
 * @p_mask[OUT]: main & sub channel mask area paramters
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_vpss_mask_get_area(const void *vi_handle, 
		struct vpss_mask_area_info *mask);

/**  
 * brief: set main & sub channel mask color
 * @vi_handle[IN]: vi module handle
 * @color[IN]: main & sub channel mask color paramters
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_vpss_mask_set_color(const void *vi_handle, 
		const struct vpss_mask_color_info *color);

/**  
 * brief: get main & sub channel mask color
 * @vi_handle[IN]: vi module handle
 * @color[OUT]: main & sub channel mask color paramters
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_vpss_mask_get_color(const void *vi_handle, 
		struct vpss_mask_color_info *color);

/********************** od *******************************/
/**
 * brief:  get occlusion detection params
 * @vi_handle[IN]: opened video input handle
 * @od[OUT]: od params 
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_od_get(const void *vi_handle, struct vpss_od_info *od);

/********************** effect *******************************/
/**  
 * ak_vpss_effect_get - get isp effect param. 
 * @vi_handle[IN]: vi module handle 
 * @type[IN]:   effect type name 
 * @effect[OUT]: effect value(s) 
 * return: 0 - success; otherwise -1;  
 */
int ak_vpss_effect_get(void *vi_handle, enum vpss_effect_type type, int *effect);

/** 
 * ak_vpss_effect_set - set isp effect param. 
 * @vi_handle[IN]: vi module handle
 * @type[IN]:   effect type name
 * @effect[IN]: effect value
 * return: 0 - success; otherwise -1; 
 */
int ak_vpss_effect_set(void *vi_handle, enum vpss_effect_type type, 
				const int effect);

/********************** ISP *******************************/
/**
 * ak_vpss_isp_init_sdk: init isp sdk
 * @void
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_init_sdk(void);

/**
 * ak_vpss_isp_exit_sdk: exit isp sdk
 * @void
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_exit_sdk(void);

/**
 * ak_vpss_isp_get_ae_attr: get AE attr
 * @vi_handle[IN]: vi module handle
 * @ae_attr[OUT]: AE attr info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_ae_attr(const void *vi_handle,
							struct vpss_isp_ae_attr *ae_attr);

/**
 * ak_vpss_isp_set_ae_attr: set AE attr
 * @vi_handle[IN]: vi module handle
 * @ae_attr[IN]: AE attr info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_ae_attr(const void *vi_handle,
							const struct vpss_isp_ae_attr *ae_attr);

/**
 * ak_vpss_isp_check_ae_stable: check ae is stable or not
 * @vi_handle[IN]: vi module handle
 * @stable[OUT]: 1 stable, 0 not stable
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_check_ae_stable(const void *vi_handle, int *stable);
/*
 * ak_vpss_isp_get_mae_attr: get MAE attr
 * @vi_handle[IN]: vi module handle
 * @mae_attr[OUT]: MAE attr info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_mae_attr(const void *vi_handle, 
		struct vpss_isp_mae_attr *mae_attr);

/**
 * ak_vpss_isp_set_mae_attr: set MAE attr
 * @vi_handle[IN]: vi module handle
 * @mae_attr[IN]: MAE attr info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_mae_attr(const void *vi_handle, 
							const struct vpss_isp_mae_attr *mae_attr);
/**
 * ak_vpss_isp_set_ae_convergence_rate: set AE convergence rate
 * @vi_handle[IN]: vi module handle
 * @value[IN]: param of convergence rate
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_ae_convergence_rate(const void *vi_handle, unsigned long value);

/**
 * ak_vpss_isp_get_ae_convergence_rate: get AE convergence rate
 * @vi_handle[IN]: vi module handle
 * @value[OUT]: param of convergence rate
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_ae_convergence_rate(const void *vi_handle, unsigned long *value);



/**
 * ak_vpss_get_weight_attr: get weight attr
 * @vi_handle[IN]: vi module handle
 * @weight_attr[OUT]: weight_attr info
 * return: 0 success, -1 failed
 */
int ak_vpss_get_weight_attr(const void *vi_handle, 
		struct vpss_isp_weight_attr *weight_attr);

/**
 * ak_vpss_set_weight_attr: set weight attr
 * @vi_handle[IN]: vi module handle
 * @weight_attr[IN]: weight_attr info
 * return: 0 success, -1 failed
 */
int ak_vpss_set_weight_attr(const void *vi_handle, 
		struct vpss_isp_weight_attr *weight_attr);

/**
 * ak_vpss_isp_get_rgb_average: get rgb average value
 * @vi_handle[IN]: vi module handle
 * @r_avr[OUT]: r average value
 * @g_avr[OUT]: g average value
 * @b_avr[OUT]: b average value
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_rgb_average(const void *vi_handle, 
        unsigned int *r_avr, unsigned int *g_avr, unsigned int *b_avr);



/**
 * ak_vpss_isp_get_wb_type: get wb type
 * @vi_handle[IN]: vi module handle
 * @wb_type[OUT]: wb type
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_wb_type(const void *vi_handle,
							struct vpss_isp_wb_type_attr *wb_type);

/**
 * ak_vpss_isp_set_wb_type: set wb type
 * @vi_handle[IN]: vi module handle
 * @wb_type[IN]: wb type
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_wb_type(const void *vi_handle,
							const struct vpss_isp_wb_type_attr *wb_type);

/**
 * ak_vpss_isp_get_3d_nr_attr: get 3D NR attr
 * @vi_handle[IN]: vi module handle
 * @nr_3d_attr[OUT]: 3D NR attr
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_3d_nr_attr(const void *vi_handle,
							struct vpss_isp_3d_nr_attr *nr_3d_attr);

/**
 * ak_vpss_isp_set_3d_nr_attr: set 3D NR attr
 * @vi_handle[IN]: vi module handle
 * @nr_3d_attr[IN]: 3D NR attr
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_3d_nr_attr(const void *vi_handle,
							const struct vpss_isp_3d_nr_attr *nr_3d_attr);

/**
 * ak_vpss_isp_get_mwb_attr: get mwb attr
 * @vi_handle[IN]: vi module handle
 * @mwb_attr[OUT]: mwb attr
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_mwb_attr(const void *vi_handle,
							struct vpss_isp_mwb_attr *mwb_attr);

/**
 * ak_vpss_isp_set_mwb_attr: set mwb attr
 * @vi_handle[IN]: vi module handle
 * @mwb_attr[IN]: mwb attr
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_mwb_attr(const void *vi_handle,
							const struct vpss_isp_mwb_attr *mwb_attr);

/**
 * ak_vpss_isp_get_awb_attr: get awb attr
 * @vi_handle[IN]: vi module handle
 * @awb_attr[OUT]: awb attr
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_awb_attr(const void *vi_handle,
							struct vpss_isp_awb_attr *awb_attr);
							
/**
 * ak_vpss_isp_set_awb_attr: set awb attr
 * @vi_handle[IN]: vi module handle
 * @awb_attr[IN]: awb attr
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_awb_attr(const void *vi_handle,
							const struct vpss_isp_awb_attr *awb_attr);

/**
 * ak_vpss_isp_get_exp_type: get exp type
 * @vi_handle[IN]: vi module handle
 * @exp_type[IN]: exp type
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_exp_type(const void *vi_handle,
							struct vpss_isp_exp_type *exp_type);

/**
 * ak_vpss_isp_set_exp_type: set exp type
 * @vi_handle[IN]: vi module handle
 * @exp_type[IN]: exp type
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_exp_type(const void *vi_handle,
							const struct vpss_isp_exp_type *exp_type);

/**
 * ak_vpss_isp_get_ae_run_info: get ae run info
 * @vi_handle[IN]: vi module handle
 * @ae_run_info[OUT]: ae run info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_ae_run_info(const void *vi_handle,
							struct vpss_isp_ae_run_info *ae_run_info);

/**
 * ak_vpss_isp_get_awb_stat_info: get awb stat info
 * @vi_handle[IN]: vi module handle
 * @awb_stat_info[OUT]: awb stat info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_awb_stat_info(const void *vi_handle, 
							struct vpss_isp_awb_stat_info *awb_stat_info);


/**
 * ak_vpss_isp_get_sensor_reg: get sensor register info
 * @vi_handle[IN]: vi module handle
 * @sensor_reg_info[IN/OUT]: sensor register info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_get_sensor_reg(const void *vi_handle,
							struct vpss_isp_sensor_reg_info *sensor_reg_info);

/**
 * ak_vpss_get_force_anti_flicker_flag: get force anti flicker flag
 * @vi_handle[IN]: vi module handle
 * return: force anti flicker flag
 */
int ak_vpss_get_force_anti_flicker_flag(const void *vi_handle);

/**
 * ak_vpss_set_force_anti_flicker_flag: set force anti flicker flag
 * @vi_handle[IN]: vi module handle
 * @force_flag[IN]: force anti flicker flag
 * return: 0 success, -1 failed
 */
int ak_vpss_set_force_anti_flicker_flag(const void *vi_handle, int force_flag);

/**
 * ak_vpss_isp_get_input_level: get day or night mode
 * @param[IN]: input param
 * return: 0 night, 1 day, -1 failed
 * notice: not suggest to use
 */
int ak_vpss_isp_get_input_level(struct ak_ir_auto_check *param);

/**
 * brief: get current lum factor 
 * return: lum factor
 * notes:
 */
int ak_vpss_isp_get_cur_lumi(void);

/**
 * brief: get night lock 
 * return: lock status
 * notes:
 */
int ak_vpss_isp_get_ir_lock(void);

/**
 * brief: get night lock time
 * return: lock status
 * notes:
 */
int ak_vpss_isp_get_ir_lock_time(void);

/**
 * ak_vpss_isp_set_auto_day_night_param: set auto day or night switch threshold
 * @param[IN]: input param threshold
 * return: NULL
 */
int ak_vpss_isp_set_auto_day_night_param(struct ak_auto_day_night_threshold *param);

/**
 * ak_vpss_isp_get_auto_day_night_level: get day or night 
 * @pre_ir_level[IN]: pre status, 0 night, 1 day
 * return: 0 night, 1 day, -1 failed
 */
int ak_vpss_isp_get_auto_day_night_level(int pre_ir_level);

/**
 * ak_vpss_isp_clean_auto_day_night_param: clean auto day or night switch threshold
 * return: NULL
 */
void ak_vpss_isp_clean_auto_day_night_param(void);

/**
 * ak_vpss_isp_set_manual_blc: set manual blc
 * @vi_handle[IN]: vi module handle
 * @mblc[IN]: manual blc info
 * return: 0 success, -1 failed
 */
int ak_vpss_isp_set_manual_blc(const void *vi_handle, 
							const struct ak_manual_blc *mblc);

/**
 * brief: open wdr
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_open_wdr(void);

/**
 * brief: close wdr
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_close_wdr(void);

/**
 * brief: get wdr attr
 * @p_wdr[OUT]: wdr attr
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vpss_get_wdr_attr(struct vpss_wdr_attr *p_wdr);

#endif
