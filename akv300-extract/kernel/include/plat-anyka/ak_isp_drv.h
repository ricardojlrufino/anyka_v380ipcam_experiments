#ifndef _AK_ISP_DRV_H_
#define _AK_ISP_DRV_H_

#include <mach-anyka/anyka_types.h>

#define ISP_DRV_LIB_VER		"ak_isp_drv_lib V3.1.7"  

enum isp_working_mode {
    ISP_JPEG_MODE,			//JPEG compression frame mode
    ISP_YUV_OUT,			//YUV single frame mode. is not support minor channel
    ISP_YUV_VIDEO_OUT,      //YUV video frame mode
    ISP_RGB_OUT,			//RGB single frame mode
    ISP_RGB_VIDEO_OUT,		//RGB video frame mode
};

enum buffer_id {
    BUFFER_ONE,
    BUFFER_TWO,
    BUFFER_THREE,
    BUFFER_FOUR,
};

typedef struct ak_isp_blc {
    T_U16     black_level_enable;	//使能位   
    T_U16     bl_r_a;				//[ 0,1023]
    T_U16     bl_gr_a;				//[ 0,1023]
    T_U16     bl_gb_a;				//[ 0,1023]
    T_U16     bl_b_a;				//[ 0,1023]
    T_S16     bl_r_offset;			//[-2048,2047]
    T_S16     bl_gr_offset;			//[-2048,2047]
    T_S16     bl_gb_offset;			//[-2048,2047]
    T_S16     bl_b_offset;			//[-2048,2047]
}AK_ISP_BLC;

typedef struct ak_isp_blc_attr {
    T_U16       blc_mode;             //0联动模式，1手动模式
    AK_ISP_BLC  m_blc;
    AK_ISP_BLC  linkage_blc[9]; 
}AK_ISP_BLC_ATTR;

typedef struct {
    T_U16 coef_b[10];    //[0,255]
    T_U16 coef_c[10];    //[0,1023]
}lens_coef;
    
typedef struct ak_isp_lsc_attr {
    T_U16       enable;
    //the reference point of lens correction
    T_U16       xref;        //[0,4096]
    T_U16       yref;        //[0,4096]
    T_U16       lsc_shift;   //[0，15]
    lens_coef   lsc_r_coef;
    lens_coef   lsc_gr_coef;
    lens_coef   lsc_gb_coef;
    lens_coef   lsc_b_coef;
    //the range of ten segment
    T_U16       range[10];   //[0，1023]
}AK_ISP_LSC_ATTR;

typedef struct ak_isp_raw_lut_attr {
    T_U16   raw_r[129];      //10bit
    T_U16   raw_g[129];      //10bit
    T_U16   raw_b[129];      //10bit
    T_U16   r_key[16];
    T_U16   g_key[16];
    T_U16   b_key[16];
    T_U16   raw_gamma_enable;
}AK_ISP_RAW_LUT_ATTR;

typedef struct ak_isp_rgb_gamma_attr {
    T_U16   r_gamma[129];   //10bit
    T_U16   g_gamma[129];   //10bit
    T_U16   b_gamma [129];  //10bit
    T_U16   r_key[16];
    T_U16   g_key[16];
    T_U16   b_key[16];
    T_U16   rgb_gamma_enable;  //如果不使能，就是一条直线
} AK_ISP_RGB_GAMMA_ATTR;

typedef struct ak_isp_y_gamma_attr {
	T_U16    ygamma[129];    //10bit
	T_U16    ygamma_key[16]; //曲线的关键点 
	T_U16    ygamma_uv_adjust_enable;
	T_U16    ygamma_uv_adjust_level;
	T_U16    ygamma_cnoise_yth1;   //Ygamma色差抑制门限值
	T_U16    ygamma_cnoise_yth2;   //Ygamma色差抑制门限值
	T_U16    ygamma_cnoise_slop;   
	T_U16    ygamma_cnoise_gain ;  //UV调整系数计算参数
}AK_ISP_Y_GAMMA_ATTR;

typedef struct ak_isp_nr1 {
    T_U16  nr1_enable;            //使能位
    T_U16  nr1_weight_rtbl[17];   //10bit
    T_U16  nr1_weight_gtbl[17];   //10bit 
    T_U16  nr1_weight_btbl[17];   //10bit
    T_U16  nr1_k;                 //[0,15]
    T_U16  nr1_lc_lut[17];        //10bit
    T_U16  nr1_lc_lut_key[16];
    T_U16  nr1_calc_g_k;
    T_U16  nr1_calc_r_k;
    T_U16  nr1_calc_b_k;
}AK_ISP_NR1;

typedef struct ak_isp_s_nr1_attr {
    T_U16       nr1_mode;             //nr1 模式，自动或者联动模式
    AK_ISP_NR1  manual_nr1;
    AK_ISP_NR1  linkage_nr1[9];       //联动参数      
}AK_ISP_NR1_ATTR;

typedef struct ak_isp_nr2 {
    T_U16    nr2_enable;
    T_U16    nr2_weight_tbl[17];    //10bit
    T_U16    nr2_k;                 //[0,15]
    T_U16    nr2_calc_y_k;
    T_U16    y_dpc_enable;
	T_U16	 y_dpc_th;
	T_U16	 y_black_dpc_enable;
	T_U16	 y_white_dpc_enable;
}AK_ISP_NR2;

typedef struct ak_isp_nr2_attr {
    T_U16       nr2_mode;          //手动或者联动模式
    AK_ISP_NR2  manual_nr2;
    AK_ISP_NR2  linkage_nr2[9];
}AK_ISP_NR2_ATTR;

typedef struct ak_isp_3d_nr_ref_attr {
	T_U32	yaddr_3d;
	T_U32	ysize_3d;
	T_U32	uaddr_3d;
	T_U32	usize_3d;
	T_U32	vaddr_3d;
	T_U32	vsize_3d;
}AK_ISP_3D_NR_REF_ATTR;

#if 0
typedef struct  ak_isp_3d_nr {
    T_U16 uv_min_enable;
    T_U16 tnr_y_enable;    
    T_U16 tnr_uv_enable;

    T_U16 mc_VAR_y_th;    		//[0, 4095]
    T_U16 t_filter_y_k;      	//[0-127]
    T_U16 s_filter_var_gain;  	//0-15
    T_U16 s_filter_var_th;    	//0-255

    T_U16 s_filter_y_th1;    	//0-1023
    T_U16 s_filter_y_th2;    	//0-1023
    T_U16 s_filter_y_k1;     	//0-128
    T_U16 s_filter_y_k2;     	//0-128
    T_U16 s_filter_y_kstep;  	//0-31

    T_U16 mc_SAD_uv_th;   		//[0, 4095]
    T_U16 t_filter_uv_k;     	//[0, 127]
    T_U16 s_filter_uv_th1;   	//[0, 128]
    T_U16 s_filter_uv_th2;   	//[0, 128]
    T_U16 s_filter_uv_k1;   	//[0,128]
    T_U16 s_filter_uv_k2;    	//[0,128] 
    T_U16 s_filter_uv_kstep; 	//[0,31] 

    T_U16 MD_th;              	//运动检测阈值 [0-127]
    T_U16 updata_ref_y;       	//更新Y
    T_U16 updata_ref_uv;		//更新uv
    T_U16 tnr_refFrame_format;	//参考帧的格式

    T_U16 tnr_skin_detect;
    T_U16 tnr_skin_k;
}AK_ISP_3D_NR;
#endif

typedef struct ak_isp_3d_nr {
	T_U16		uv_min_enable;
	T_U16		tnr_y_enable;	
	T_U16		tnr_uv_enable;
	T_U16		updata_ref_y;			//更新Y
	T_U16		updata_ref_uv;			//更新uv
	T_U16		tnr_refFrame_format;	//参考帧的格式
	T_U16		tnr_t_y_ex_k_cfg;//lz0499 9_6
	T_U16		y_2dnr_enable;
	T_U16		uv_2dnr_enable;
    
	T_U16		uvnr_k; 		//[0, 15]
	T_U16		uvlp_k; 		//[0, 15]
	T_U16		t_uv_k; 		//[0, 127]
	T_U16		t_uv_minstep;	//[0,31]
	T_U16		t_uv_mf_th1;	//[0, 8191]
	T_U16		t_uv_mf_th2;	//[0, 8191]
	T_U16		t_uv_diffth_k1; //[0, 255]
	T_U16		t_uv_diffth_k2; //[0, 255]
	T_U16		t_uv_diffth_slop;//[0, 255]
	T_U16		t_uv_mc_k;		//[0-31]
	T_U16		t_uv_ac_th; 	//[0, 1023]
	
	T_U16		ynr_weight_tbl[17];
	T_U16		ynr_calc_k;
	T_U16		ynr_k;			//[0, 15]
	T_U16		ynr_diff_shift; //[0,1]
	T_U16		ylp_k;			//[0, 15]
	T_U16		t_y_th1;		//[0, 255]
	T_U16		t_y_k1; 		//[0, 127]
	T_U16		t_y_k2; 		//[0, 127]
	T_U16		t_y_kslop;		//[0, 127]
	T_U16		t_y_minstep;	//[0-31]
	T_U16		t_y_mf_th1; 	//[0, 8191]
	T_U16		t_y_mf_th2; 	//[0, 8191]
	T_U16		t_y_diffth_k1;	//[0, 255]
	T_U16		t_y_diffth_k2;	//[0, 255]
	T_U16		t_y_diffth_slop;//[0, 255]
	T_U16		t_y_mc_k;		//[0-31]
	T_U16		t_y_ac_th;	    //[0, 1023]
	
	T_U32		md_th;			//[0, 65535]  运动检测阈值 [0-127]
}AK_ISP_3D_NR;

typedef struct  ak_isp_3d_nr_attr {
    T_U16         _3d_nr_mode;  
    AK_ISP_3D_NR  manual_3d_nr;
    AK_ISP_3D_NR  linkage_3d_nr[9];
}AK_ISP_3D_NR_ATTR;

typedef struct  ak_isp_3d_nr_stat_info {
	T_U16		MD_stat_max;	
	T_U16		MD_stat[24][32];		//运动检测分块输出lz0499 9_12
	T_U32		MD_level;			    //运动检测输出    
}AK_ISP_3D_NR_STAT_INFO;

typedef struct ak_isp_gb {

    T_U16   gb_enable;        //使能位
    T_U16   gb_en_th;         //[0,255]
    T_U16   gb_kstep;         //[0,15]
    T_U16   gb_threshold;     //[0,1023
} AK_ISP_GB;
    
typedef struct ak_isp_gb_attr {
    T_U16      gb_mode;       //模式选择，手动或者联动
    AK_ISP_GB  manual_gb;
    AK_ISP_GB  linkage_gb[9];
} AK_ISP_GB_ATTR;

typedef struct ak_isp_demo_attr {
    T_U16  dm_HV_th;       //方向判别系数
    T_U16  dm_rg_thre;     //[0 1023] 
    T_U16  dm_bg_thre;     //[0 1023]
    T_U16  dm_hf_th1;      //[0, 1023]
    T_U16  dm_hf_th2;      //[0, 1023]

    T_U16  dm_rg_gain;     //[0 255]
    T_U16  dm_bg_gain;     //[0 255]
    T_U16  dm_gr_gain;     //[0 255]
    T_U16  dm_gb_gain;     //[0 255]
    //T_U16  cfa_mode;
}AK_ISP_DEMO_ATTR;

typedef struct ak_isp_ccm {
	T_U16  cc_enable;		   //颜色校正使能
	T_U16  cc_cnoise_yth;	   //亮度控制增益
	T_U16  cc_cnoise_gain;	   //亮度控制增益
	T_U16  cc_cnoise_slop;	   //亮度控制增益
	T_S16  ccm[3][3];		   //[-2048, 2047]    
}AK_ISP_CCM;

typedef struct ak_isp_ccm_attr {
    T_U16       cc_mode;  		//颜色校正矩阵联动或者手动
    AK_ISP_CCM  manual_ccm; 
    AK_ISP_CCM  ccm[4]; 		//四个联动矩阵
}AK_ISP_CCM_ATTR;

typedef struct ak_isp_wdr {
    T_U16   hdr_uv_adjust_level;        //uv调整程度, [0,31]
    T_U16   hdr_cnoise_suppress_slop;   //抑制斜率  
    T_U16   wdr_enable;
  
	T_U16	wdr_th1;	  //0-1023
	T_U16	wdr_th2;	  //0-1023
	T_U16	wdr_th3;	  //0-1023
	T_U16	wdr_th4;	  //0-1023
	T_U16	wdr_th5;	  //0-1023
    
    //T_U16 wdr_light_weight;
    
	T_U16	area_tb1[65];	  //曲线 10bit
	T_U16	area_tb2[65];	  //曲线 10bit
	T_U16	area_tb3[65];	  //曲线 10bit
	T_U16	area_tb4[65];	  //曲线 10bit
	T_U16	area_tb5[65];	  //曲线 10bit
	T_U16	area_tb6[65];	  //曲线 10bit
    
	T_U16	area1_key[16];
	T_U16	area2_key[16];
	T_U16	area3_key[16];
	T_U16	area4_key[16];
	T_U16	area5_key[16];
	T_U16	area6_key[16];
    
    T_U16   hdr_uv_adjust_enable;       //uv调整使能
    T_U16   hdr_cnoise_suppress_yth1;   //色彩噪声亮度阈值1           
    T_U16   hdr_cnoise_suppress_yth2;   //色彩噪声亮度阈值2
    T_U16   hdr_cnoise_suppress_gain;   //色差抑制
}AK_ISP_WDR;

typedef struct ak_isp_wdr_attr {
	T_U16 wdr_mode;              //模式选择，手动或者联动
	AK_ISP_WDR manual_wdr;
	AK_ISP_WDR linkage_wdr[9]; 
}AK_ISP_WDR_ATTR;

typedef struct ak_isp_wdr_ex_attr { 
	 T_U16     hdr_blkW;
	 T_U16     hdr_blkH;
	 T_U16     hdr_reverseW_g;   	 //[0,511]
	 T_U16     hdr_reverseW_shift;   //[0,7];
	 T_U16     hdr_reverseH_g;       //[0,511]
	 T_U16     hdr_reverseH_shift;   //[0,7]
	 T_U16     hdr_weight_g;       	 //[0,511]
	 T_U16     hdr_weight_shift;     //[0,7]
	 T_U16     hdr_weight_k;    	 //[0,15]
}AK_ISP_WDR_EX_ATTR;
    
typedef struct ak_isp_sharp {

    T_U16   mf_hpf_k;                  	//[0,127]
    T_U16   mf_hpf_shift;             	//[0,15]

    T_U16   hf_hpf_k;                 	//[0,127]
    T_U16   hf_hpf_shift;            	//[0,15]

    T_U16   sharp_method;            	//[0,3]      
    T_U16   sharp_skin_gain_weaken;  	//[0，3]

    T_U16   sharp_skin_gain_th;      	//[0, 255]
    T_U16   sharp_skin_detect_enable;
    T_U16   ysharp_enable;          	//[0,1]
    T_S16   MF_HPF_LUT[256];        	//[-256,255]
    T_S16   HF_HPF_LUT[256]; 			//[-256,255]
    T_U16	MF_LUT_KEY[16];
	T_U16	HF_LUT_KEY[16];
}AK_ISP_SHARP;
    
typedef struct ak_isp_sharp_attr{   
    T_U16          ysharp_mode;
    AK_ISP_SHARP   manual_sharp_attr;
    AK_ISP_SHARP   linkage_sharp_attr[9];   
}AK_ISP_SHARP_ATTR;

typedef struct ak_isp_sharp_ex_attr {    
    T_S16  mf_HPF[6];            //   
                                 //M13,M14,M15,
                                 //M11,M12,M14,
                                 //M10,M11,M13,
    T_S16  hf_HPF[3];            //
                                 //M22 M21,M22,
                                 //M21,M20, M21,
    T_U16   sharp_skin_max_th;     //[0, 255]
    T_U16   sharp_skin_min_th;     //[0, 255]
    T_U16   sharp_skin_v_max_th;  //[0, 255]
    T_U16   sharp_skin_v_min_th;  //[0, 255]
    T_U16   sharp_skin_y_max_th;  //[0, 255]
    T_U16   sharp_skin_y_min_th;  //[0, 255]
    //U16   mf_hpf_k;
    //U16   sharp_method;
}AK_ISP_SHARP_EX_ATTR;

typedef struct ak_isp_saturation {
    T_U16    SE_enable;       // 使能位
    T_U16    SE_th1;          //[0, 1023]
    T_U16    SE_th2;          //[0, 1023]
	T_U16	 SE_th3;	      //[0, 1023]
	T_U16	 SE_th4;	      //[0, 1023]
	T_U16	 SE_scale_slop1;  //[0, 255]
	T_U16	 SE_scale_slop2;  //[0, 255]
	T_U16	 SE_scale1; 	  //[0,255]
	T_U16	 SE_scale2;	      //[0,255]
	T_U16	 SE_scale3; 	  //[0,255]    
}AK_ISP_SATURATION;

typedef struct  ak_isp_saturation_attr {
    T_U16               SE_mode;        //饱和度模式
    AK_ISP_SATURATION   manual_sat;
    AK_ISP_SATURATION   linkage_sat[9];   
}AK_ISP_SATURATION_ATTR;
 
typedef struct  ak_isp_contrast {
     T_U16  y_contrast;  	//[0,511]
     T_S16  y_shift;    	//[0, 511]
}AK_ISP_CONTRAST;

typedef struct ak_isp_auto_contrast
{
	T_U16  dark_pixel_area; 	//[0, 511]
	T_U16  dark_pixel_rate;  	//[1, 256]
	T_U16  shift_max;    		//[0, 127]
}AK_ISP_AUTO_CONTRAST;

typedef struct  ak_isp_contrast_ATTR {
	T_U16	cc_mode;	//模式选择，手动或者联动
	AK_ISP_CONTRAST	manual_contrast;
	AK_ISP_AUTO_CONTRAST	linkage_contrast[9];
}AK_ISP_CONTRAST_ATTR;

typedef  struct ak_isp_fcs {
    T_U16  fcs_th;     			//[0, 255]
    T_U16  fcs_gain_slop;  		//[0,63]
    T_U16  fcs_enable;   		//使能位
    T_U16  fcs_uv_nr_enable;  	//使能位
    T_U16  fcs_uv_nr_th;  		//[0, 1023]
}AK_ISP_FCS;

typedef  struct ak_isp_fcs_attr {
    T_U16       fcs_mode;   	//模式选择，手动或者联动
    AK_ISP_FCS  manual_fcs;
    AK_ISP_FCS  linkage_fcs[9];
}AK_ISP_FCS_ATTR;

typedef struct ak_isp_hue {
	T_U16  hue_sat_en;			//hue使能
	T_S8   hue_lut_a[65];		//[-128, 127]
	T_S8   hue_lut_b[65];		//[-128, 127]
	T_U8   hue_lut_s[65];		//[0, 255]
}AK_ISP_HUE;

typedef struct ak_isp_hue_attr {
	T_U16        hue_mode;		//联动或者手动
	AK_ISP_HUE   manual_hue; 
	AK_ISP_HUE   hue[4];		//四个联动参数
}AK_ISP_HUE_ATTR;

typedef  struct ak_isp_rgb2yuv_attr {
    T_U16  mode; 				//bt601 或者bt709
}AK_ISP_RGB2YUV_ATTR;

typedef struct ak_isp_effect_attr {
    T_U16  y_a;     // [0, 255]
    T_S16  y_b;     //[-128, 127]
    T_S16  uv_a;    //[-256, 255]
    T_S16  uv_b;    //[-256, 255]
    T_U16  dark_margin_en;   //黑边使能
}AK_ISP_EFFECT_ATTR;

typedef struct ak_isp_ddpc { 
	T_U16 ddpc_enable;         //动态坏点使能位
	T_U16 ddpc_th;             //10bit
	T_U16 white_dpc_enable;    //白点消除使能位
	T_U16 black_dpc_enable;    //黑点消除使能位
}AK_ISP_DDPC;
 
typedef struct ak_isp_ddpc_attr {
	T_U16 ddpc_mode;            //模式选择，手动或者联动
	AK_ISP_DDPC manual_ddpc;
	AK_ISP_DDPC linkage_ddpc[9]; 
}AK_ISP_DDPC_ATTR;

typedef struct ak_isp_sdpc_attr { 
    T_U32   sdpc_enable; 		//静态坏点使能位

    /* 静态坏点坐标值，最大为1024个
    数据格式{6h0 ,y_position[9:0],5'h0,x_position[10:0]} */
    T_U32   sdpc_table[1024];
}AK_ISP_SDPC_ATTR;

typedef struct ak_isp_af_attr {
	T_U16  af_win0_left; 	//[0, 1024] lz0499 9_6
	T_U16  af_win0_right;	//[0, 1024]
	T_U16  af_win0_top;	    //[0, 768]
	T_U16  af_win0_bottom;   //[0, 768]

	T_U16  af_win1_left; 	//[0, 1024]
	T_U16  af_win1_right;	//[0, 1024]
	T_U16  af_win1_top;	    //[[0, 768]
	T_U16  af_win1_bottom;   //[0, 768]

	T_U16  af_win2_left; 	//[0, 1024]
	T_U16  af_win2_right;	//[0, 1024]
	T_U16  af_win2_top;	    //[0, 768]
	T_U16  af_win2_bottom;   //[0, 768]

	T_U16  af_win3_left; 	//[0, 1024]
	T_U16  af_win3_right;	//[0, 1024]
	T_U16  af_win3_top;	    //[0, 768]
	T_U16  af_win3_bottom;   //[0, 768]

	T_U16  af_win4_left; 	//[0, 1024]
	T_U16  af_win4_right;	//[0, 1024]
	T_U16  af_win4_top;	    //[0, 768]
	T_U16  af_win4_bottom;   //[0, 768]
    
	T_U16   af_th;       //[0, 128]
}AK_ISP_AF_ATTR;

typedef struct ak_isp_af_stat_info {
   T_U32  af_statics[5];            //统计结果  
}AK_ISP_AF_STAT_INFO;

typedef struct ak_isp_weight_attr {
   T_U16   zone_weight[8][16];      //权重系数  
}AK_ISP_WEIGHT_ATTR;

typedef  struct  ak_isp_wb_type_attr {
    T_U16   wb_type;    
}AK_ISP_WB_TYPE_ATTR;

typedef struct  ak_isp_awb_algo {
    short current_r_gain;
    short current_b_gain;
    short current_g_gain;
    short target_r_gain;
    short target_b_gain;
    short target_g_gain;
    short calc_r_gain;
    short calc_b_gain;
    short calc_g_gain;
}AK_ISP_AWB_ALGO;

typedef  struct  ak_isp_mwb_attr {
    T_U16  r_gain;
    T_U16  g_gain;
    T_U16  b_gain;
    T_S16  r_offset;
    T_S16  g_offset;
    T_S16  b_offset;
}AK_ISP_MWB_ATTR;

typedef  struct ak_isp_awb_attr {   
    T_U16   g_weight[16];
    T_U16   y_low;               	//y_low<=y_high
    T_U16   y_high;
		T_U16   err_est;
    T_U16   gr_low[10];            //gr_low[i]<=gr_high[i]
    T_U16   gb_low[10];            //gb_low[i]<=gb_high[i]
    T_U16   gr_high[10];
    T_U16   gb_high[10];
    T_U16   rb_low[10];           //rb_low[i]<=rb_high[i]
    T_U16   rb_high[10];

    //awb软件部分需要设置的参数
    T_U16   auto_wb_step;                 //白平衡步长计算
    T_U16   total_cnt_thresh;            //像素个数阈值
    T_U16   colortemp_stable_cnt_thresh; //稳定帧数，多少帧一样认为环境色温改变
    T_U16	colortemp_envi[10];
}AK_ISP_AWB_ATTR;

typedef  struct  ak_isp_awb_ctrl {
	int rgain_max;
	int rgain_min;
	int ggain_max;
	int ggain_min;
	int bgain_max;
	int bgain_min;
	int rgain_ex;
	int bgain_ex;
}AK_ISP_AWB_CTRL;

typedef  struct  ak_isp_awb_ex_attr {
	int awb_ex_ctrl_enable;
	AK_ISP_AWB_CTRL awb_ctrl[10];
}AK_ISP_AWB_EX_ATTR;

typedef  struct ak_isp_awb_stat_info {
    //白平衡统计结果
    T_U32   total_R[10];
    T_U32   total_G[10];
    T_U32   total_B[10];
    T_U32   total_cnt[10];
    //经由自动白平衡算法算出的白平衡增益值
    T_U16   r_gain;
    T_U16   g_gain;
    T_U16   b_gain;
    T_S16   r_offset;
    T_S16   g_offset;
    T_S16   b_offset;
    T_U16   current_colortemp_index;     //环境色温标记，是参数随环境变化的色温指标。
    T_U16   colortemp_stable_cnt[10];         //每一种色温稳定的帧数计数
}AK_ISP_AWB_STAT_INFO;

typedef struct ak_isp_wb_gain {
    T_U16  r_gain;
    T_U16  g_gain;
    T_U16  b_gain;
    T_S16  r_offset;
    T_S16  g_offset;
    T_S16  b_offset;
}AK_ISP_WB_GAIN;

typedef struct ak_isp_raw_hist_attr {
    T_U16  enable;
}AK_ISP_RAW_HIST_ATTR;

typedef struct ak_isp_raw_hist_stat_info {
    T_U32   raw_g_hist[256];
    T_U32   raw_g_total;
}AK_ISP_RAW_HIST_STAT_INFO;

typedef struct ak_isp_rgb_hist_attr {
    T_U16  enable;
}AK_ISP_RGB_HIST_ATTR;

typedef struct ak_isp_rgb_hist_stat_info { 
    T_U32   rgb_hist[256];
    T_U32   rgb_total;
}AK_ISP_RGB_HIST_STAT_INFO;

typedef struct  ak_isp_yuv_hist_attr {
    T_U16  enable;
}AK_ISP_YUV_HIST_ATTR;

typedef struct  ak_isp_yuv_hist_stat_info {
    T_U32    y_hist[256];
    T_U32    y_total;
}AK_ISP_YUV_HIST_STAT_INFO;
    
typedef  struct  ak_isp_exp_type {
    T_U16  exp_type; 
}AK_ISP_EXP_TYPE;

typedef struct ak_isp_manual_ae {
	T_U32   exp_time;			 			
    T_U32   a_gain; 
	T_U32   d_gain; 
	T_U32   isp_d_gain;  
}AK_ISP_MAE_ATTR;

typedef struct ak_isp_ae_attr {
    T_U32   exp_time_max;			//曝光时间的最大值
    T_U32   exp_time_min;			//曝光时间的最小值
    T_U32   d_gain_max;      		//数字增益的最大值
    T_U32   d_gain_min;     		//数字增益的最小值
    T_U32   isp_d_gain_min;  		//isp数字增益的最小
    T_U32   isp_d_gain_max;  		//isp数字增益的最大值
    T_U32   a_gain_max;     		//模拟增益的最大值
    T_U32   a_gain_min;      		//模拟增益的最小值     
    T_U32   exp_step;            	//用户曝光调整步长
    T_U32   exp_stable_range;     	//稳定范围
    T_U32   target_lumiance;     	//目标亮度
    T_U32   envi_gain_range[10][2];
    T_U32   hist_weight[16];
    T_U32		OE_suppress_en;
    T_U32   OE_detect_scope;		//[0,255]
    T_U32   OE_rate_max;			//[0, 255]
    T_U32   OE_rate_min;			//[0, 255]
}AK_ISP_AE_ATTR;
    
typedef  struct ak_isp_ae_run_info {
    T_U8   current_calc_avg_lumi;         		//现在的计算出的亮度值
    T_U8   current_calc_avg_compensation_lumi; 	//经过曝光补偿后的亮度值

    T_U8   current_darked_flag;                 //白天黑夜的标记

    T_S32  current_a_gain;                     	//模拟增益的值
    T_S32  current_d_gain;                    	//数字增益的值
    T_S32  current_isp_d_gain;                	//isp数字增益的值
    T_S32  current_exp_time;                	//曝光时间的值

    T_U32  current_a_gain_step;             	//现在的模拟增益的步长
    T_U32  current_d_gain_step;             	//数字增益的步长
    T_U32  current_isp_d_gain_step;        		//isp数字增益的步长
    T_U32  current_exp_time_step;          		//曝光时间的步长
}AK_ISP_AE_RUN_INFO;
    
typedef struct ak_isp_aec_algo {
    T_U32  aec_status;       		//自动曝光控制的状态，稳定或者不稳定  
    T_U32  aec_locked;      		//自动曝光锁定与否
    
    T_U8  exp_time_need_updata;   	//需要被更新，减少iic读写次数         
    T_U8  a_gain_need_updata;    	//需要被更新，减少iic读写次数
    T_U8  d_gain_need_updata;       //需要被更新，只有需要更新时才写
    T_U8  isp_d_gain_need_updata; 	//需要被更新
    T_U8  lock_range;
}AK_ISP_AEC_ALGO;

//帧率控制结构体
typedef  struct ak_isp_frame_rate_attr {
	T_U32  	hight_light_frame_rate ;
	T_U32  	hight_light_max_exp_time ;
	T_U32  	hight_light_to_low_light_gain;
	T_U32  	low_light_frame_rate;
	T_U32  	low_light_max_exp_time;
	T_U32  	low_light_to_hight_light_gain;
}AK_ISP_FRAME_RATE_ATTR;

typedef struct ak_isp_misc_attr {
	T_U16  hsyn_pol;
	T_U16  vsync_pol;
	T_U16  pclk_pol;
	T_U16  test_pattern_en;
	T_U16  test_pattern_cfg;
	T_U16  cfa_mode;
	T_U16  inputdataw;
	T_U16  one_line_cycle;
	T_U16  hblank_cycle;
	T_U16  frame_start_delay_en;
	T_U16  frame_start_delay_num;
	T_U16  flip_en;
	T_U16  mirror_en;
	T_U16  twoframe_merge_en;//lz0499 9_12
	T_U16  mipi_line_end_sel; //lz0499 9_21
	T_U16  mipi_line_end_cnt_en_cfg; //lz0499 9_21
	T_U16  mipi_count_time; //lz0499 9_21
} AK_ISP_MISC_ATTR; 

typedef struct ak_isp_mask_area {
    T_U16  start_xpos;
    T_U16  end_xpos;
    T_U16  start_ypos;
    T_U16  end_ypos;
    T_BOOL enable;
}AK_ISP_MASK_AREA;

typedef struct ak_isp_mask_area_attr {
    AK_ISP_MASK_AREA  mask_area[8];
}AK_ISP_MASK_AREA_ATTR;

typedef struct ak_isp_main_chan_mask_area_attr {
    AK_ISP_MASK_AREA  mask_area[4];
}AK_ISP_MAIN_CHAN_MASK_AREA_ATTR;

typedef struct ak_isp_sub_chan_mask_area_attr {
    AK_ISP_MASK_AREA  mask_area[4];
}AK_ISP_SUB_CHAN_MASK_AREA_ATTR;

typedef struct ak_isp_mask_color_attr {
    T_U8  color_type;   //0:指定遮挡色；1：
    T_U8  mk_alpha;
    T_U8  y_mk_color;
    T_U8  u_mk_color;
    T_U8  v_mk_color;
}AK_ISP_MASK_COLOR_ATTR;

//enum for sub sample
typedef enum {
    SUBSMP_1X,  /*no sub sample*/
    SUBSMP_2X,  /*sub sample 1/2 * 1/2 */
    SUBSMP_4X,  /*sub sample 1/4 * 1/4 */
    SUBSMP_8X   /*sub sample 1/8 * 1/8 */ 
} T_SUBSMP_RTO;

typedef enum mask_num{
    MAIN_CHAN_ONE=0,
    MAIN_CHAN_TWO,
    MAIN_CHAN_THREE,
    MAIN_CHAN_FOURE,
    SUB_CHAN_ONE,
    SUB_CHAN_TWO,
    SUB_CHAN_THREE,
    SUB_CHAN_FOURE,
}MASK_NUM;

typedef enum osd_channel {
	OSD_CHN0 = 0,
	OSD_CHN1,
	OSD_CHN2,
	OSD_CHN_NUM
} OSD_CHANNEL;

typedef struct ak_isp_osd_color_table_attr {
	T_U32 color_table[16];
}AK_ISP_OSD_COLOR_TABLE_ATTR;


// lz0499 9_26 have some problem need to fix
typedef struct ak_isp_osd_context_attr {
	OSD_CHANNEL chn;
	T_U32 *osd_context_addr;
	T_U32 osd_width;
	T_U32 osd_height;
	T_U16 start_xpos;
	T_U16 start_ypos;
	T_U16 alpha;
	T_BOOL enable;
}AK_ISP_OSD_CONTEXT_ATTR;

typedef struct ak_isp_osd_mem_attr {
	OSD_CHANNEL chn;
	T_U8 *dma_paddr;
	T_U8 *dma_vaddr;
	T_U32 size;			
}AK_ISP_OSD_MEM_ATTR;

typedef enum ak_isp_pclk_polar {
	POLAR_ERR	= 0,
	POLAR_RISING,
	POLAR_FALLING,
}AK_ISP_PCLK_POLAR;

int ak_isp_vi_start_capturing(void);
int ak_isp_vi_stop_capturing(void);
int ak_isp_vi_set_input_size(int width, int height);
int ak_isp_vi_set_crop(int sx, int sy, int width, int height);
int ak_isp_vi_apply_mode(enum isp_working_mode);
int ak_isp_vo_set_misc_attr(AK_ISP_MISC_ATTR *p_misc);
int ak_isp_vo_get_misc_attr(AK_ISP_MISC_ATTR *p_misc);
int ak_isp_vo_get_inputdataw(void);

int ak_isp_set_flip_mirror(int flip_en, int mirror_en);
void ak_isp_get_flip_mirror(int * flip_en, int * mirror_en, int * height_block_num);

int ak_isp_vo_set_main_channel_scale(int width, int height);
int ak_isp_vo_get_main_channel_scale(int *width, int *height);

int ak_isp_vo_set_sub_channel_scale(int width, int height);
int ak_isp_vo_get_sub_channel_scale(int *width, int *height);

int ak_isp_vo_check_irq_status (void);
int ak_isp_vo_clear_irq_status(int bit);
int ak_isp_vo_enable_irq_status(int bit);
int ak_isp_vo_set_cfg_reg(int regaddr, int value, int bitmask);

int ak_isp_vo_enable_buffer(enum buffer_id id);
int ak_isp_vo_disable_buffer(enum buffer_id id);
int ak_isp_vo_set_buffer_addr(enum buffer_id id,
		unsigned long yaddr_main_chan_addr, unsigned long yaddr_sub_chan_addr/*,unsigned long yaddr_sub_chan3_addr*/);
int ak_isp_vo_get_using_frame_buf_id(void);
int ak_isp_vo_update_setting(void);
int ak_isp_is_continuous(void);

//int ak_isp_vpp_set_osd(AK_ISP_OSD_ATTR *p_osd);

//int ak_isp_vpp_set_occlusion_attr(AK_ISP_OCCLUSION_ATTR *p_occ);
//int ak_isp_vpp_occlusion_color_attr(AK_ISP_OCCLUSION_COLOR *p_occ_color);

/**
 * @brief: set blc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_blc:  blc param
 */
int ak_isp_vp_set_blc_attr(AK_ISP_BLC_ATTR *p_blc);

/**
 * @brief: get blc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_blc:  blc param
 */
int ak_isp_vp_get_blc_attr(AK_ISP_BLC_ATTR *p_blc);

/**
 * @brief: set lsc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_lsc:  lsc param
 */
int ak_isp_vp_set_lsc_attr(AK_ISP_LSC_ATTR *p_lsc);

/**
 * @brief: get lsc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_lsc:  lsc param
 */
int ak_isp_vp_get_lsc_attr(AK_ISP_LSC_ATTR *p_lsc);

/**
 * @brief: set rgb gamma param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_rgb_gamma:  rgb gamma param
 */
int ak_isp_vp_set_rgb_gamma_attr(AK_ISP_RGB_GAMMA_ATTR *p_rgb_gamma);

/**
 * @brief: get rgb gamma param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_rgb_gamma:  rgb gamma param
 */
int ak_isp_vp_get_rgb_gamma_attr(AK_ISP_RGB_GAMMA_ATTR *p_rgb_gamma);

/**
 * @brief: set y gamma param
 * @author: lz
 * @date: 2016-8-26
 * @param [in] *p_y_gamma:  y gamma param
 */
int ak_isp_vp_set_y_gamma_attr(AK_ISP_Y_GAMMA_ATTR *p_y_gamma);

/**
 * @brief: get y gamma param
 * @author: lz
 * @date: 2016-8-26
 * @param [out] *p_y_gamma:  y gamma param
 */
int ak_isp_vp_get_y_gamma_attr(AK_ISP_Y_GAMMA_ATTR *p_y_gamma);

/**
 * @brief: set raw gamma param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_raw_lut:  raw gamma param
 */
int ak_isp_vp_set_raw_lut_attr(AK_ISP_RAW_LUT_ATTR *p_raw_lut);

/**
 * @brief: get raw gamma param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_raw_lut:  raw gamma param
 */
int ak_isp_vp_get_raw_lut_attr(AK_ISP_RAW_LUT_ATTR *p_raw_lut);

/**
 * @brief: set dpc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_dpc:  dpc param
 */
int ak_isp_vp_set_dpc_attr(AK_ISP_DDPC_ATTR *p_dpc);

/**
 * @brief: get dpc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_dpc:  dpc param
 */
int ak_isp_vp_get_dpc_attr(AK_ISP_DDPC_ATTR *p_dpc);

/**
 * @brief: set sdpc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_sdpc:  sdpc param
 */
int ak_isp_vp_set_sdpc_attr(AK_ISP_SDPC_ATTR *p_sdpc);

/**
 * @brief: get sdpc param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_sdpc:  sdpc param
 */
int ak_isp_vp_get_sdpc_attr(AK_ISP_SDPC_ATTR *p_sdpc);

/**
 * @brief: set raw noise remove param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *nr1:  raw noise remove  param
 */
int ak_isp_vp_set_nr1_attr(AK_ISP_NR1_ATTR *p_nr1);

/**
 * @brief: get raw noise remove param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *nr1:  raw noise remove  param
 */
int ak_isp_vp_get_nr1_attr(AK_ISP_NR1_ATTR *p_nr1);

/**
 * @brief: set green balance param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_gb:  green balance param
 */
int ak_isp_vp_set_gb_attr(AK_ISP_GB_ATTR *p_gb);

/**
 * @brief: get green balance param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_gb:  green balance param
 */
int ak_isp_vp_get_gb_attr(AK_ISP_GB_ATTR *p_gb);

/**
 * @brief: set demosaic param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_demo:  demosaic param
 */
int ak_isp_vp_set_demo_attr(AK_ISP_DEMO_ATTR *p_demo);

/**
 * @brief: get demosaic param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_demo:  demosaic param
 */
int ak_isp_vp_get_demo_attr(AK_ISP_DEMO_ATTR *p_demo);

/**
 * @brief: set color correct param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_ccm:  color correct param
 */
int ak_isp_vp_set_ccm_attr(AK_ISP_CCM_ATTR *p_ccm);

/**
 * @brief: set color correct param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_ccm:  color correct param
 */
int ak_isp_vp_get_ccm_attr(AK_ISP_CCM_ATTR *p_ccm);

/**
 * @brief: set wdr  param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_wdr: wdr param
 */
int ak_isp_vp_set_wdr_attr(AK_ISP_WDR_ATTR *p_wdr);

/**
 * @brief: get wdr  param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_wdr: wdr param
 */
int ak_isp_vp_get_wdr_attr(AK_ISP_WDR_ATTR *p_wdr);

/**
 * @brief: set yuv noise remove param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_nr2: noise remove param
 */
int ak_isp_vp_set_nr2_attr(AK_ISP_NR2_ATTR *p_nr2);

/**
 * @brief: get yuv noise remove param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_nr2: noise remove param
 */
int ak_isp_vp_get_nr2_attr(AK_ISP_NR2_ATTR *p_nr2);

/**
 * @brief: set 3d noise remove param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_3d_nr: 3d noise remove param
 */
int ak_isp_vp_set_3d_nr_attr(AK_ISP_3D_NR_ATTR *p_3d_nr);

/**
 * @brief: get 3d noise remove param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_3d_nr: 3d noise remove param
 */
int ak_isp_vp_get_3d_nr_attr(AK_ISP_3D_NR_ATTR *p_3d_nr);

/**
 * @brief: set 3d noise remove reference param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_ref: 3d noise remove referenc param
 */
int ak_isp_vp_set_3d_nr_ref_addr(AK_ISP_3D_NR_REF_ATTR *p_ref);

/**
 * @brief: get 3d noise remove reference param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_ref: 3d noise remove referenc param
 */
int ak_isp_vp_get_3d_nr_ref_addr(AK_ISP_3D_NR_REF_ATTR *p_ref);

/**
 * @brief: get 3d noise remove statics param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_3d_nr_stat_info: 3d noise remove statics param
 */
int ak_isp_vp_get_3d_nr_stat_info(AK_ISP_3D_NR_STAT_INFO * p_3d_nr_stat_info);

/**
 * @brief: set sharp param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_sharp: sharp param
 */
int ak_isp_vp_set_sharp_attr(AK_ISP_SHARP_ATTR *p_sharp);

/**
 * @brief: get sharp param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_sharp: sharp param
 */
int ak_isp_vp_get_sharp_attr(AK_ISP_SHARP_ATTR* p_sharp);

/**
 * @brief: set sharp other param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_sharp_ex: sharp other param
 */
int ak_isp_vp_set_sharp_ex_attr(AK_ISP_SHARP_EX_ATTR *p_sharp_ex);

/**
 * @brief: get sharp other param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_sharp_ex: sharp other param
 */
int ak_isp_vp_get_sharp_ex_attr(AK_ISP_SHARP_EX_ATTR* p_sharp_ex);

/**
 * @brief: set false color param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *fcs: false color param
 */
int ak_isp_vp_set_fcs_attr(AK_ISP_FCS_ATTR *p_fcs);

/**
 * @brief: get false color param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *fcs: false color param
 */
int ak_isp_vp_get_fcs_attr(AK_ISP_FCS_ATTR *p_fcs);

/**
 * @brief: set hue param
 * @author: lz
 * @date: 2016-8-26
 * @param [in] *p_hue:hue param
 */
int ak_isp_vp_set_hue_attr(AK_ISP_HUE_ATTR *p_hue);

/**
 * @brief: gethue param
 * @author: lz
 * @date: 2016-8-26
 * @param [in] *p_hue:hue param
 */
int ak_isp_vp_get_hue_attr(AK_ISP_HUE_ATTR *p_hue);

/**
 * @brief: set satruration param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_sat: satruration param
 */
int ak_isp_vp_set_saturation_attr(AK_ISP_SATURATION_ATTR *p_sat);

/**
 * @brief: get satruration param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_sat: satruration param
 */
int ak_isp_vp_get_saturation_attr(AK_ISP_SATURATION_ATTR *p_sat);

int ak_isp_vp_set_contrast_attr(AK_ISP_CONTRAST_ATTR  *p_contrast);
int ak_isp_vp_get_contrast_attr(AK_ISP_CONTRAST_ATTR  *p_contrast);

int ak_isp_vp_set_rgb2yuv_attr(AK_ISP_RGB2YUV_ATTR*p_rgb2yuv_attr);
int ak_isp_vp_get_rgb2yuv_attr(AK_ISP_RGB2YUV_ATTR*p_rgb2yuv_attr);
int ak_isp_vp_set_effect_attr(AK_ISP_EFFECT_ATTR *p_effect_attr);
int ak_isp_vp_get_effect_attr(AK_ISP_EFFECT_ATTR *p_effect_attr);

int ak_isp_vpp_set_mask_area_attr(AK_ISP_MASK_AREA_ATTR *p_mask, MASK_NUM num);
int ak_isp_vpp_get_mask_area_attr(AK_ISP_MASK_AREA_ATTR *p_mask,  MASK_NUM num);
int ak_isp_vpp_set_mask_color(AK_ISP_MASK_COLOR_ATTR *p_mask);
int ak_isp_vpp_get_mask_color(AK_ISP_MASK_COLOR_ATTR *p_mask);
int ak_isp_vpp_set_main_chan_mask_area_attr(AK_ISP_MAIN_CHAN_MASK_AREA_ATTR *p_mask);
int ak_isp_vpp_get_main_chan_mask_area_attr(AK_ISP_MAIN_CHAN_MASK_AREA_ATTR *p_mask);
int ak_isp_vpp_set_sub_chan_mask_area_attr(AK_ISP_SUB_CHAN_MASK_AREA_ATTR *p_mask);
int ak_isp_vpp_get_sub_chan_mask_area_attr(AK_ISP_SUB_CHAN_MASK_AREA_ATTR *p_mask);


int ak_isp_vpp_set_osd_color_table_attr(AK_ISP_OSD_COLOR_TABLE_ATTR *p_isp_color_table);
int ak_isp_vpp_set_main_channel_osd_context_attr(AK_ISP_OSD_CONTEXT_ATTR *p_context);
int ak_isp_vpp_set_sub_channel_osd_context_attr(AK_ISP_OSD_CONTEXT_ATTR *p_context);


int ak_isp_vpp_set_main_channel_osd_mem_attr(AK_ISP_OSD_MEM_ATTR *p_mem);
int ak_isp_vpp_set_sub_channel_osd_mem_attr(AK_ISP_OSD_MEM_ATTR *p_mem);



/**
 * @brief: set auto focus param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_af:  af  param
 */
int ak_isp_vp_set_af_attr(AK_ISP_AF_ATTR *p_af);

int ak_isp_vp_set_af_win34_attr( AK_ISP_AF_ATTR *p_af);

/**
 * @brief: get auto focus param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_af:  af  param
 */
int ak_isp_vp_get_af_attr(AK_ISP_AF_ATTR *p_af);

/**
 * @brief: get auto focus statics info param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_af_stat_info:  af_stat  param
 */
int ak_isp_vp_get_af_stat_info(AK_ISP_AF_STAT_INFO *p_af_stat_info);

/**
 * @brief: set white balance  type
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_type:  white balance  type  param
 */
int ak_isp_vp_set_wb_type(AK_ISP_WB_TYPE_ATTR *p_type);
/**
 * @brief: get white balance  type
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_type:  white balance  type  param
 */
int ak_isp_vp_get_wb_type(AK_ISP_WB_TYPE_ATTR *p_type);

/**
 * @brief: set manual white balance
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_mwb:  manual white balance  type  param
 */
int ak_isp_vp_set_mwb_attr(AK_ISP_MWB_ATTR *p_mwb);

/**
 * @brief: get manual white balance
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_mwb:  manual white balance  type  param
 */
int ak_isp_vp_get_mwb_attr(AK_ISP_MWB_ATTR *p_mwb);

/**
 * @brief: set auto white balance
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_awb:  auto white balance  param
 */
int ak_isp_vp_set_awb_attr(AK_ISP_AWB_ATTR *p_awb);

/**
 * @brief: get auto white balance
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_awb:  auto white balance  param
 */
int ak_isp_vp_get_awb_attr(AK_ISP_AWB_ATTR *p_awb);

/**
 * @brief: set auto white balance ex attr
 * @author: wyf
 * @date: 2016-5-16
 * @param [in] *p_awb:  auto white balance  param
 */
int ak_isp_vp_set_awb_ex_attr(AK_ISP_AWB_EX_ATTR *p_awb);

/**
 * @brief: get auto white balance ex attr
 * @author: wyf
 * @date: 2016-5-16
 * @param [in] *p_awb:  auto white balance  param
 */
int ak_isp_vp_get_awb_ex_attr(AK_ISP_AWB_EX_ATTR *p_awb);

/**
 * @brief: get awb statics info
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_awb_stat_info:  awb statics info  param
 */
int ak_isp_vp_get_awb_stat_info(AK_ISP_AWB_STAT_INFO *p_awb_stat_info);

/**
 * @brief: set exp  type
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_exp_type:  exp type  param
 */
int ak_isp_vp_set_exp_type(AK_ISP_EXP_TYPE* p_exp_type);

/**
 * @brief: get exp  type
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_exp_type:  exp type  param
 */
int ak_isp_vp_get_exp_type( AK_ISP_EXP_TYPE* p_exp_type);


/**
 * @brief: set manual exposure  param
 * @author: lizhi
 * @date: 2020-06-01
 * @param [in] *p_mae:  manual exposure  param
 */
int ak_isp_vp_set_mae_attr( AK_ISP_MAE_ATTR *p_mae);


/**
 * @brief: get manual exposure  param
 * @author: lizhi
 * @date: 2020-06-01
 * @param [in] *p_mae:  manual exposure  param
 */
int  ak_isp_vp_get_mae_attr(AK_ISP_MAE_ATTR *p_mae);


/**
 * @brief: set auto exposure  param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_ae:  auto exposure  param
 */
int ak_isp_vp_set_ae_attr( AK_ISP_AE_ATTR *p_ae);

/**
 * @brief: get auto exposure  param
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_ae:  auto exposure  param
 */
int ak_isp_vp_get_ae_attr(AK_ISP_AE_ATTR *p_ae);

/**
 * @brief: set frame rate param
 * @author: lz
 * @date: 2016-8-29
 * @param [in] *p_frame_rate:  frame rate param
 */
int ak_isp_vp_set_frame_rate( AK_ISP_FRAME_RATE_ATTR*p_frame_rate);

/**
 * @brief: set frame rate param
 * @author: lz
 * @date: 2016-8-29
 * @param [in] *p_frame_rate:  frame rate param
 */
int ak_isp_vp_get_frame_rate(AK_ISP_FRAME_RATE_ATTR*p_frame_rate);

/**
 * @brief: get auto  exposure  running info 
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_ae_stat:  auto  exposure  running info  param
 */
int ak_isp_vp_get_ae_run_info(AK_ISP_AE_RUN_INFO*p_ae_stat);

/**
 * @brief: get raw hist   running info 
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_raw_hist_stat:  raw hist info  param
 */
int ak_isp_vp_get_raw_hist_stat_info(AK_ISP_RAW_HIST_STAT_INFO *p_raw_hist_stat);

/**
 * @brief: get rgb hist   running info 
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_rgb_hist_stat:  rgb hist info  param
 */
int ak_isp_vp_get_rgb_hist_stat_info(AK_ISP_RGB_HIST_STAT_INFO *p_rgb_hist_stat);

/**
 * @brief: get rgb hist   running info 
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [out] *p_yuv_hist_stat:  yuv hist info  param
 */
int ak_isp_vp_get_yuv_hist_stat_info(AK_ISP_YUV_HIST_STAT_INFO *p_yuv_hist_stat);

int ak_isp_vp_set_rgb_hist_attr(AK_ISP_RGB_HIST_ATTR *p_rgb_hist);
int ak_isp_vp_get_rgb_hist_attr(AK_ISP_RGB_HIST_ATTR *p_rgb_hist);


int ak_isp_vp_set_yuv_hist_attr(AK_ISP_YUV_HIST_ATTR *p_yuv_hist);
int ak_isp_vp_get_yuv_hist_attr(AK_ISP_YUV_HIST_ATTR *p_yuv_hist);

/**
 * @brief: set zone weight param 
 * @author: xiepenghe
 * @date: 2016-5-06
 * @param [in] *p_weight:  weight  param
 */
int ak_isp_vp_set_zone_weight(AK_ISP_WEIGHT_ATTR *p_weight);
int ak_isp_vp_get_zone_weight( AK_ISP_WEIGHT_ATTR *p_weight);
int ak_isp_vp_set_raw_hist_attr(AK_ISP_RAW_HIST_ATTR *p_raw_hist);
int ak_isp_vp_get_raw_hist_attr(AK_ISP_RAW_HIST_ATTR *p_raw_hist);

/* Pclk_Polar not isp function, but described in PG */
AK_ISP_PCLK_POLAR ak_isp_get_pclk_polar(void);

int ak_isp_set_isp_capturing(int resume);

int ak_isp_irq_work(void);
int ak_isp_ae_work(void);
int ak_isp_awb_work(void);

typedef void (*ISPDRV_CB_PRINTK)(char * format, ...);
typedef void (*ISPDRV_CB_MEMCPY)(void *dst, void *src, unsigned long sz);
typedef void (*ISPDRV_CB_MEMSET)(void *ptr, unsigned char value, unsigned long sz);
typedef void* (*ISPDRV_CB_MALLOC)(unsigned long sz);
typedef void (*ISPDRV_CB_FREE)(void *ptr);
typedef void* (*ISPDRV_CB_DMAMALLOC)(unsigned long sz, void *handle);
typedef void (*ISPDRV_CB_DMAFREE)(void *ptr, unsigned long sz, unsigned long handle);
typedef void (*ISPDRV_CB_MSLEEP)(int ms);

typedef struct {
    ISPDRV_CB_PRINTK cb_printk;
    ISPDRV_CB_MEMCPY cb_memcpy;
    ISPDRV_CB_MEMSET cb_memset;
    ISPDRV_CB_MALLOC cb_malloc;
    ISPDRV_CB_FREE   cb_free;
    ISPDRV_CB_DMAMALLOC cb_dmamalloc;
    ISPDRV_CB_DMAFREE cb_dmafree;
    ISPDRV_CB_MSLEEP cb_msleep;
}AK_ISP_FUNC_CB; 

typedef struct sensor_reg_info {
    T_U16 reg_addr;
    T_U16 value;
}AK_ISP_SENSOR_REG_INFO;

typedef struct sensor_init_para {
    T_U16 num;
    AK_ISP_SENSOR_REG_INFO *reg_info;
}AK_ISP_SENSOR_INIT_PARA;

enum sensor_bus_type {
	BUS_TYPE_RAW,
	BUS_TYPE_YUV,
	BUS_TYPE_NUM
};

enum scene {
	SCENE_INDOOR = 0,
	SCENE_OUTDOOR
};

typedef struct sensor_callback {
    int (*sensor_init_func)(const AK_ISP_SENSOR_INIT_PARA *para);
    int (*sensor_read_reg_func)(const int reg_addr);
    int (*sensor_write_reg_func)(const int reg_addr, int value);
    int (*sensor_read_id_func)(void);	//no use IIC bus
	int (*sensor_update_a_gain_func)(const unsigned int a_gain);
	int (*sensor_update_d_gain_func)(const unsigned int d_gain);
	int (*sensor_updata_exp_time_func)(unsigned int exp_time);
	int (*sensor_timer_func)(void);

	int (*sensor_probe_id_func)(void);	//use IIC bus
	int (*sensor_get_resolution_func)(int *width, int *height);
	int (*sensor_get_mclk_func)(void);
	int (*sensor_get_fps_func)(void);
	int (*sensor_get_valid_coordinate_func)(int *x, int *y);
	enum sensor_bus_type (*sensor_get_bus_type_func)(void);
	int (*sensor_get_parameter_func)(int param, void *value);

	int (*sensor_set_power_on_func)(const int pwdn_pin, const int reset_pin);
	int (*sensor_set_power_off_func)(const int pwdn_pin, const int reset_pin);
	int (*sensor_set_fps_func)(const int fps);
	int (*sensor_set_standby_in_func)(const int pwdn_pin, const int reset_pin);
	int (*sensor_set_standby_out_func)(const int pwdn_pin, const int reset_pin);
}AK_ISP_SENSOR_CB;

int  isp2_module_init(AK_ISP_FUNC_CB *cb, AK_ISP_SENSOR_CB *sensor_cb, void *reg_base);
void isp2_module_fini(void);

void  isp2_print_reg_table(void);

int ak_isp_register_sensor(void *sensor_info);
void *ak_isp_get_sensor(int *index);
void ak_isp_remove_all_sensors(void);
int ak_isp_set_td(void);
int ak_isp_reload_td(void);
enum scene ak_isp_get_scene(void);
int ak_isp_get_iso(void);


int ak_isp_vpp_mainchn_osdmem_useok(void);

int ak_isp_vpp_subchn_osdmem_useok(void);

int ak_isp_get_yuvaddr_and_mdinfo(enum buffer_id  id, void **yuv, void **mdinfo);

#endif
