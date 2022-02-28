#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include "ak_common.h"
#include "ak_thread.h"
#include "ak_vi.h"
#include "ak_ipc_srv.h"
#include "ak_venc.h"
#include "ak_drv_irled.h"
#include "ak_drv_ir.h"
#include "ak_drv_ptz.h"
#include "ak_vpss.h"
#include "ak_mt.h"

#define SAVE_PATH        	"/mnt/video_encode"	//default save path

/* ctrl handle */
struct video_handle {
	void *venc_handle;					//video encode handle
	void *stream_handle;				//video stream handle
	enum encode_group_type enc_type;	//current encode type
	ak_pthread_t ak_venc_tid;			//thread id
};

/* resolation define */
struct resolution_t {
	unsigned int width;
	unsigned int height;
	unsigned char str[20];
};

struct mt_ctrl_info {
	void *mt_handle;
	unsigned char run_flag;
    ak_pthread_t mt_tid;
	void *vi_handle;
	MT_POINT cur_angle;
};

static struct mt_ctrl_info mt_ctrl = {0};

/* encode this number frames, main index , sub index */
static int enc_count, work_nr; //global resolution index
static void *video_in_handle = NULL;			//global vi handle
struct video_handle ak_venc[ENCODE_GRP_NUM];	//handle array
char txt_name[40] = "1.txt";
int day_night = 0;



/* usage */
static void mt_usage(const char *pro)
{
	/* current save patch is settled: /mnt/video_encode */
	ak_print_normal("Usage: %s frame_number filename\n", pro);
	ak_print_notice("e.g: %s 1000 %s 0(0 day, 1 night)\n", pro, txt_name);
	ak_print_notice("e.g: %s 1000 %s 0\n", pro, txt_name);
	ak_print_normal("    It will capture 1000 frames and store in %s \n",
	    SAVE_PATH);
}

/* create file to save stream */
static void *mt_venc_open_file(int enc_type)
{
	char file[128] = {0};
	char time_str[32] = {0};
	const char *suffix[2] = {".str", ".jpg"};
	struct ak_date date;

	/* get time string */
	ak_get_localdate(&date);
	ak_date_to_string(&date, time_str);

	/* distinguish stream and jpeg */
	if (enc_type < ENCODE_PICTURE) {
		sprintf(file, "%s/%s_%d_%s", SAVE_PATH, time_str, enc_type, suffix[0]);
		ak_print_normal_ex("[ H264 ] save file: %s\n", file);
	} else {
		static int jpg_cnt = 0;

		sprintf(file, "%s/%s_%d_%s", SAVE_PATH, time_str, jpg_cnt, suffix[1]);
		ak_print_normal_ex("[ JPG ] save file: %s\n", file);
		jpg_cnt++;
	}

	/* open for all mode */
	FILE *fp = NULL;
	fp = fopen(file, "a");
	if (!fp) {
		ak_print_error_ex("open [%s] failed, %s\n", file, strerror(errno));
		return NULL;
	}

	return fp;
}

/* save data */
static void mt_venc_save_data(FILE *filp, unsigned char *data, int len)
{
	int ret = len;
	if (filp) {
		/* make sure all data were writ to file */
		do {
			ret -= fwrite(data, 1, ret, filp);
		} while (ret > 0);
	}
}

static int init_irled(void)
{
	struct ak_drv_irled_hw_param param;

	param.irled_working_level = 1;
	return ak_drv_irled_init(&param);
}

/* open video encode handle */
static void *mt_demo_open_encoder(int index)
{
	struct encode_param param = {0};

	switch (index) {
	case 0:
		param.width = 1920;
		param.height = 1080;
		param.minqp = 20;
		param.maxqp = 51;
		param.fps = 25;
		param.goplen = param.fps * 2;	   //current gop is stationary
		param.bps = 2000;				   //kbps
		param.profile = PROFILE_MAIN;	   //main profile
		param.use_chn = ENCODE_SUB_CHN;   //use main yuv channel data
		param.enc_grp = ENCODE_RECORD;     //assignment from enum encode_group_type
		param.br_mode = BR_MODE_CBR;	   //default is cbr
		param.enc_out_type = H264_ENC_TYPE;//h.264
		break;
	case 1:
		param.width = 1920;
		param.height = 1080;
		param.minqp = 20;
		param.maxqp = 51;
		param.fps = 10;
		param.goplen = param.fps * 2;
		param.bps = 500;	//kbps
		param.profile = PROFILE_MAIN;		//same as above
		param.use_chn = ENCODE_MAIN_CHN;
		param.enc_grp = ENCODE_MAINCHN_NET;	//just this scope difference
		param.br_mode = BR_MODE_CBR;
		param.enc_out_type = H264_ENC_TYPE;
		break;
	case 2:
		param.width = 640;
		param.height = 480;
		param.minqp = 20;
		param.maxqp = 51;
		param.fps = 10;
		param.goplen = param.fps * 2;
		param.bps = 300;	//kbps
		param.profile = PROFILE_MAIN;
		param.use_chn = ENCODE_SUB_CHN;		//use sub yuv channel data
		param.enc_grp = ENCODE_SUBCHN_NET;	//same as above
		param.br_mode = BR_MODE_CBR;
		param.enc_out_type = H264_ENC_TYPE;
		break;
	case 3:
		param.width = 640;
		param.height = 480;
		param.minqp = 20;
		param.maxqp = 51;
		param.fps = 10;
		param.goplen = param.fps * 2;
		param.bps = 500;	//kbps
		param.profile = PROFILE_MAIN;
		param.use_chn = ENCODE_SUB_CHN;
		param.enc_grp = ENCODE_PICTURE;		//jpeg encode
		param.br_mode = BR_MODE_CBR;
		param.enc_out_type = MJPEG_ENC_TYPE;	//jpeg encode
		break;
	default:
		return NULL;
		break;
	}

	return ak_venc_open(&param);
}

/*
 * venc_stop_stream - stop specific group's encode
 * grp[IN]: group index
 */
static void mt_stop_venc_stream(enum encode_group_type grp)
{
	struct video_handle *enc = &ak_venc[grp];

	if (!enc->ak_venc_tid)
		return;
	/* wait thread exit */
	ak_thread_join(enc->ak_venc_tid);

	/* release resource */
	ak_venc_cancel_stream(enc->stream_handle);
	enc->stream_handle = NULL;
	ak_venc_close(enc->venc_handle);
	enc->venc_handle = NULL;
}

/*
 * venc_save_stream_thread - save stream to file
 */
static void *mt_save_venc_stream_thread(void *arg)
{
	long int tid = ak_thread_get_tid();
	ak_print_normal_ex("start a work thread, tid: %ld\n", tid);

	struct video_handle *handle = (struct video_handle *)arg;
	struct video_channel_attr attr = {{0}};  // code_updating
	int md_v_size = 0;
	
	ak_vi_get_channel_attr(video_in_handle, &attr); // code_updating

	if (0 == attr.res[VIDEO_CHN_MAIN].height % VPSS_MD_DIMENSION_V_MAX)
		md_v_size = VPSS_MD_DIMENSION_V_MAX;
	else
		md_v_size = 16;

	/* first, open file */
	FILE *fp = mt_venc_open_file(handle->enc_type);
	ak_print_error_ex("########### open ###########\n");
	int looptimes = enc_count;

	/*
	 * according number of total save frame, iterate
	 * specific times
	 */

	struct vpss_md_info md = {0}; // code_updating	
	//RECTANGLE rect; // code_updating	
	int v,h; // code_updating
	//ak_mt_init(); // code_updating
	FILE *fw = fopen(txt_name, "a+"); // code_updating
	
	while (looptimes > 0) {				
		
		struct video_stream stream = {0};
		/* get stream */
		int ret = ak_venc_get_stream(handle->stream_handle, &stream);
		if (ret) {
			ak_sleep_ms(10);
			continue;
		}
		ak_print_normal_ex("[tid: %ld, chn: %d] get stream, size: %d, last: %d\n",
				tid, handle->enc_type, stream.len, looptimes);

		/*
		 * save to file,
		 * Notice:
		 * 		on your project, replace your code to do
		 * 		your things here.
		 */
		mt_venc_save_data(fp, stream.data, stream.len);


		ak_vpss_md_get_stat(video_in_handle, &md);// code_updating	
		
		//ak_mt_mrd(md.stat, VPSS_MD_DIMENSION_V_MAX, &rect, 1, 768);
		//fprintf(fw, "%d %d %d %d\n", rect.pt.px, rect.pt.py, rect.height, rect.width);
	//	printf("rect information: %d %d %d %d\n", rect.pt.px, rect.pt.py, rect.height, rect.width);
		fprintf(fw,"ak_vpss_md_get_stat starts!\n"); // code_updating	
		for(v=0; v<md_v_size ; v++)// code_updating	
		{		
			for(h=0; h<MD_SIZE; h++)// code_updating		
			{			
				fprintf(fw, "%d\n", md.stat[v][h]);// code_updating 	
			}	
		}	
		fprintf(fw,"ak_vpss_md_get_stat ends!\n");// code_updating	
		

		/* release frame */
		ret = ak_venc_release_stream(handle->stream_handle, &stream);
		looptimes--;

		if ((looptimes % 3) == 0)
			ak_sleep_ms(10);
	}

	/* when write done, close file */
	if (fp)
	{
		fclose(fp);
		fclose(fw);// code_updating
	}

	/* stop video encode */
	mt_stop_venc_stream(handle->enc_type);
	work_nr--;
	
	ak_print_normal_ex("### thread id: %ld exit ###\n", ak_thread_get_tid());
	ak_thread_exit();

	return NULL;
}

/*
 * mt_init_vi - init video input module,
 * 							after this, you can init video encode
 * return: 0 on success, -1 failed
 * notes: if first isp.cfg match ok, second path is useless.
 */
static int mt_init_vi(void)
{
	int ret = -1;								//return value
	struct video_channel_attr attr;				//vi channel attribute
	const char *cfg = "/etc/jffs2/";			//isp config file storage path
	
	attr.crop.left = 0;
	attr.crop.top  = 0;
	attr.crop.width = 1920;
	attr.crop.height = 1080;

	attr.res[VIDEO_CHN_MAIN].width = 1920;
	attr.res[VIDEO_CHN_MAIN].height = 1080;
	attr.res[VIDEO_CHN_SUB].width = 640;
	attr.res[VIDEO_CHN_SUB].height = 360;

	attr.res[VIDEO_CHN_MAIN].max_width = 1920;
	attr.res[VIDEO_CHN_MAIN].max_height = 1080;
	attr.res[VIDEO_CHN_SUB].max_width = 640;
	attr.res[VIDEO_CHN_SUB].max_height = 480;

	/* 
	 * match sensor
	 * the location of isp config can either a file or a directory 
	 */
	ret = ak_vi_match_sensor(cfg);
	if (ret) {
		ak_print_normal("match sensor failed\n");	
		return -1;
	}

	/* 
	 * open video input device
	 */
	video_in_handle = ak_vi_open(VIDEO_DEV0);
	if (NULL == video_in_handle) {
		ak_print_normal("vi device open failed\n");	
		return -1;
	}

	/* 
	 * set vi working parameters 
	 * default parameters: 25fps, day mode, auto frame-control
	 */
	ret = ak_vi_set_channel_attr(video_in_handle, &attr);
	if (ret) {
		ak_vi_close(video_in_handle);
		return -1;
	}

	if (day_night) {
		init_irled();
		ak_drv_irled_set_working_stat(1);
		ak_drv_ir_set_ircut(1);
		ak_vi_switch_mode(video_in_handle, VI_MODE_NIGHT);
	}

	/* 
	 * start capture frames
	 */
	ret = ak_vi_capture_on(video_in_handle);
	if (ret) {
		ak_vi_close(video_in_handle);
		return -1;
	}

	return 0;
}

static void mt_close_vi(void)
{
	ak_print_normal("closing video input ...\n");
	ak_vi_capture_off(video_in_handle);
	ak_vi_close(video_in_handle);
	video_in_handle = NULL;
}


/*
 * stream flow to get and save data
 */
static int mt_start_venc_stream(enum encode_group_type grp)
{
	struct video_handle *handle = &ak_venc[grp];

	ak_print_normal_ex("stream encode mode, start encode group: %d\n", grp);

	/* init encode handle */
	handle->venc_handle = mt_demo_open_encoder(grp);
	if (!handle->venc_handle) {
		ak_print_error_ex("video encode open type: %d failed\n", grp);
		return -1;
	}

	/* request stream, video encode module will start capture and encode */
	handle->stream_handle = ak_venc_request_stream(video_in_handle,
			handle->venc_handle);
	if (!handle->stream_handle) {
		ak_print_error_ex("request stream failed\n");
		return -1;
	}
	handle->enc_type = grp;
	/* create thread to get stream and do you primary mission */
	ak_thread_create(&handle->ak_venc_tid, mt_save_venc_stream_thread,
			(void *)handle, 100*1024, 90);
	work_nr++;

	return 0;
}

static void wait_ptz_turn(void)
{
	enum ptz_status status[PTZ_DEV_NUM];
	
	do{
		ak_sleep_ms(5);
		ak_drv_ptz_get_status(PTZ_DEV_H, &status[PTZ_DEV_H]);
		ak_drv_ptz_get_status(PTZ_DEV_V, &status[PTZ_DEV_V]);
	} while((status[PTZ_DEV_H] != PTZ_INIT_OK)
	    || (status[PTZ_DEV_V] != PTZ_INIT_OK));
}

static void* mt_ctrl_thread(void* param)
{
	ak_print_normal_ex("thread id: %ld\n", ak_thread_get_tid());
	ak_thread_set_name("dana_mt");
	struct vpss_md_info md_info = {0};
	MT_POINT angle = {0};
	int ret = -1;
	MT_RECTANGLE rect = {{0}};
	int sensor_fps = 25;
	struct video_channel_attr attr = {{0}};  // code_updating
	int md_v_size = 0;
	
	ak_vi_get_channel_attr(mt_ctrl.vi_handle, &attr); // code_updating
	int vid_height = attr.res[VIDEO_CHN_MAIN].height; // code_updating
	int vid_width = attr.res[VIDEO_CHN_MAIN].width; // code_updating


	if (0 == vid_height % VPSS_MD_DIMENSION_V_MAX)
		md_v_size = VPSS_MD_DIMENSION_V_MAX;
	else
		md_v_size = 16;

	ak_mt_set_motion_region_params(mt_ctrl.mt_handle, md_v_size, 10, 300, 10, 8);

	while (mt_ctrl.run_flag) {
		/*get isp 3d stat info*/
		ret = ak_vpss_md_get_stat(mt_ctrl.vi_handle, &md_info);
		if (0 != ret) {
			ak_print_error_ex("get 3d nr stat info fail\n");
			break;
		} 

		ret = ak_mt_tracking(mt_ctrl.mt_handle, md_info.stat, md_v_size, &rect, &angle);

		ak_vi_set_box_rect((rect.pt.px - 1) * vid_width / MD_SIZE,
					(rect.pt.py - 1) * vid_height / md_v_size, 
					rect.width * vid_width / MD_SIZE,
					rect.height * vid_height / md_v_size);
		
		if (ret) {
			ak_print_normal_ex("angle.px = %d, angle.py = %d, ret = %d\n", angle.px, angle.py, ret);
			ak_mt_wait_camera_rotation(mt_ctrl.mt_handle);

			ak_drv_ptz_turn_to_pos(angle.px, angle.py);
			wait_ptz_turn();	
			
			mt_ctrl.cur_angle.px = angle.px;
			mt_ctrl.cur_angle.py = angle.py;
			ak_mt_update_camera_angle(mt_ctrl.mt_handle, mt_ctrl.cur_angle.px, mt_ctrl.cur_angle.py);
		}		

		sensor_fps = ak_vi_get_fps(mt_ctrl.vi_handle);
		ak_mt_set_fps(mt_ctrl.mt_handle, sensor_fps);
		ak_mt_set_max_invalid_frames(mt_ctrl.mt_handle, 10 * sensor_fps);
		if (25 == sensor_fps) {	//day high light
			ak_mt_set_max_decision_frames(mt_ctrl.mt_handle, 7);	
			ak_sleep_ms(40);
		} else {	// night, day low light
			ak_mt_set_max_decision_frames(mt_ctrl.mt_handle, 5);
			ak_sleep_ms(100);
		}
	}

	ak_print_normal_ex("### thread id: %ld exit ###\n", ak_thread_get_tid());
	ak_thread_exit();
    return NULL;
}


static int mt_init(void *vi_handle)
{
	if (mt_ctrl.run_flag) {
		return AK_SUCCESS;
	}
	
	mt_ctrl.mt_handle = ak_mt_init();
	
	if (NULL == mt_ctrl.mt_handle)
		return AK_FAILED;

	//ak_mt_set_motion_region_params(mt_ctrl.mt_handle, VPSS_MD_DIMENSION_V_MAX, 10, 300, 10, 8);
	ak_mt_set_camera_rotation_range(mt_ctrl.mt_handle, 0, 350, 0, 130);
	ak_mt_set_camera_init_angle(mt_ctrl.mt_handle, 180, 65);
	ak_mt_set_mrd_filters(mt_ctrl.mt_handle, 12000, 6000);
	ak_mt_set_wait_time(mt_ctrl.mt_handle, 200);

	mt_ctrl.run_flag = AK_TRUE;
	mt_ctrl.vi_handle = vi_handle;
	mt_ctrl.cur_angle.px = 180;
	mt_ctrl.cur_angle.py = 65;

	int ret = ak_thread_create(&(mt_ctrl.mt_tid), mt_ctrl_thread,
        NULL, ANYKA_THREAD_MIN_STACK_SIZE, -1);
	if(ret){
		ak_print_normal_ex("create dana_mt_ctrl_thread failed, ret=%d\n", ret);
	} else {
		ak_print_normal_ex("dana mt control start\n");
	}

	return ret;
}


static void mt_exit(void)
{
	if (mt_ctrl.run_flag) {
		mt_ctrl.run_flag = AK_FALSE;
		ak_thread_join(mt_ctrl.mt_tid);
		ak_print_notice_ex("dana_mt_ctrl_thread join OK\n");
		ak_mt_destroy(mt_ctrl.mt_handle);
		mt_ctrl.mt_handle = NULL;
	}
}

static int mt_ptz_init(void)
{
	/* open ptz*/
	if (ak_drv_ptz_open())
		return -1;

    /* change motor param if you changed your machine mould */
	ak_drv_ptz_set_angle_rate(24/24.0, 21/21.0);
	ak_drv_ptz_set_degree(350, 130);
	ak_drv_ptz_check_self(PTZ_FEEDBACK_PIN_NONE);
	return 0;
}

int main(int argc, char **argv)
{
	ak_print_normal("------------ mt demo start ------------\n");

	if (argc != 4) {
		mt_usage(argv[0]);
		return -1;
	}

	if (access(SAVE_PATH, W_OK) != 0) {
		if (mkdir(SAVE_PATH, 0755)) {
			ak_print_error_ex("mkdir: %s, %s\n", SAVE_PATH, strerror(errno));
			return -1;
		}
	}

	strcpy(txt_name, SAVE_PATH);
	strcat(txt_name, "/");
	strcat(txt_name, argv[2]);
	enc_count = atoi(argv[1]);
	ak_print_normal("\tencode %d frames\n", enc_count);

	day_night = atoi(argv[3]);

	/* init vi */
	if (mt_init_vi()) {
		ak_print_normal_ex("vi init failed\n");
		return -1;
	}

	/* init ptz */
	if (mt_ptz_init()){
		ak_print_normal_ex("ptz init failed\n");
		goto MT_DEMO_EXIT;
	}

	/* init mt */
	if (mt_init(video_in_handle)) {
		ak_print_normal_ex("mt init failed\n");
		goto MT_DEMO_EXIT;
	}

	/* start venc to save stream for debug*/
	mt_start_venc_stream(ENCODE_MAINCHN_NET);

	while (work_nr > 0)
		ak_sleep_ms(100);
	
MT_DEMO_EXIT:
	/* exit mt */
	mt_exit();

	/* close ptz*/
	ak_drv_ptz_close();

	/* close vi */
	mt_close_vi();

	ak_print_normal("mt demo exit ...\n");

	return 0;
}

