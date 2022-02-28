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

#ifdef AK_RTOS
#include "command.h"
#include "kernel.h"
#endif

#define SAVE_PATH        	"/mnt/sdcard/CAM/record"	//default save path
#define FIRST_ISP_PATH 	"/mnt/sdcard/CAM/isp"		//first config path
#define BACK_ISP_PATH 		"/mvs/local"		       //sub config path

#define INVALID_INDEX		(0xffffffff)	//invalid index value
#define E_VIDEO_SIZE_MAX 	(12)			//max group

#define SINGLE_FRAME_USE_CHANNEL (VIDEO_CHN_SUB)

#define VENC_PICTURE_TEST	0

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

#ifdef AK_RTOS
static char *help[]={
    "uvc module demo",
    "usage:uvcdemo [yuv/mjpeg]\n"
};
#endif

/* encode this number frames, main index , sub index */
static int enc_count, main_index, sub_index, work_nr; //global resolution index
static void *video_in_handle = NULL;			//global vi handle
struct video_handle ak_venc[ENCODE_GRP_NUM];	//handle array

/* resolution assignment pool */
static struct resolution_t resolutions[E_VIDEO_SIZE_MAX] = {
	{176,   144,   "E_VIDEO_SIZE_QCIF"},        //0
	{320,   180,   "E_VIDEO_SIZE_180P"},
	{320,   240,   "E_VIDEO_SIZE_QVGA"},
	{352,   288,   "E_VIDEO_SIZE_CIF"},
	{640,   360,   "E_VIDEO_SIZE_360P"},
	{640,   480,   "E_VIDEO_SIZE_VGA"},         //5
	{720,   576,   "E_VIDEO_SIZE_PAL"},
	{720,   480,   "E_VIDEO_SIZE_NTSC"},
	{1280,  720,   "E_VIDEO_SIZE_720P"},
	{1280,  800,   "E_VIDEO_SIZE_WXGA"},
	{1280,  960,   "E_VIDEO_SIZE_960P"},        //10
	{1920,	1080,  "E_VIDEO_SIZE_1080P"},
};

/* usage */
static void venc_usage(const char *pro)
{
	/* current save patch is settled: /mnt/video_encode */
	ak_print_normal("Usage: %s frame_number main_res sub_res\n", pro);
	ak_print_notice("e.g: %s 1000 main:[6, 11] sub[0, 5]\n", pro);

	int i = 0;
	for (i=0; i<E_VIDEO_SIZE_MAX; ++i) {
	    if (i <= 5) {
	        ak_print_normal("    sub_res:  index=%.2d, %s,\t[%d,%d]\n",
	            i, resolutions[i].str,
	            resolutions[i].width, resolutions[i].height);
	    } else {
	        ak_print_normal("    main_res: index=%.2d, %s,\t[%d,%d]\n",
	            i, resolutions[i].str,
	            resolutions[i].width, resolutions[i].height);
	    }
	}

	ak_print_notice("e.g: %s 1000 10 5\n", pro);
	ak_print_normal("    It will capture 1000 frames and store in %s\n",
	    SAVE_PATH);
}

/* signal handle callback */
static void venc_process_signal(int sig)
{
	ak_print_notice("\t signal %d caught", sig);
	if ((SIGTERM == sig) || (SIGINT == sig) || (SIGSEGV == sig)) {
		exit(EXIT_FAILURE);
	}
}

/* register signal handle callback */
static int venc_register_signal(void)
{
	signal(SIGSEGV, venc_process_signal);
	signal(SIGINT, venc_process_signal);
	signal(SIGTERM, venc_process_signal);
	signal(SIGCHLD, SIG_IGN);

	return 0;
}

/* create file to save stream */
static void *venc_open_file(int enc_type)
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
static void venc_save_data(FILE *filp, unsigned char *data, int len)
{
	int ret = len;
	if (filp) {
		/* make sure all data were writ to file */
		do {
			ret -= fwrite(data, 1, ret, filp);
		} while (ret > 0);
	}
}

/* set capture resolution */
static int venc_set_resolution(void *handle)
{
	struct video_resolution resolution = {0};
	/* get origin resolution */
	if (ak_vi_get_sensor_resolution(handle, &resolution))
		ak_print_error_ex("get sensor resolution failed\n");
	else
		ak_print_normal("\tcurrent sensor resolution: [W: %d], [H: %d]\n",
				resolution.width, resolution.height);

	/* set channel attribute */
	struct video_channel_attr attr;
	memset(&attr, 0, sizeof(struct video_channel_attr));

	attr.res[VIDEO_CHN_MAIN].width = resolutions[main_index].width;
	attr.res[VIDEO_CHN_MAIN].height = resolutions[main_index].height;
	attr.res[VIDEO_CHN_SUB].width = resolutions[sub_index].width;
	attr.res[VIDEO_CHN_SUB].height = resolutions[sub_index].height;

	// HACK inverted for compatiblity with older version of precompiled library
	attr.res[VIDEO_CHN_SUB].max_width = 1280;
	attr.res[VIDEO_CHN_SUB].max_height = 720;
	attr.res[VIDEO_CHN_MAIN].max_width = 640;
	attr.res[VIDEO_CHN_MAIN].max_height = 480;
	
	attr.crop.left = 0;
	attr.crop.top = 0;
	attr.crop.width = resolutions[main_index].width;
	attr.crop.height = resolutions[main_index].height;

	ak_print_error_ex("mw: %d, mh: %d, sw: %d, sh: %d\n",
		attr.res[VIDEO_CHN_MAIN].width, attr.res[VIDEO_CHN_MAIN].height,
		attr.res[VIDEO_CHN_SUB].width, attr.res[VIDEO_CHN_SUB].height);

	/* modify resolution */
	if (ak_vi_set_channel_attr(handle, &attr)) {
		ak_print_error_ex("set channel attribute failed\n");
		return -1;
	}

	/* after set resolution, save channal attribute info */
	struct video_channel_attr cur_attr;
	memset(&cur_attr, 0, sizeof(struct video_channel_attr));
	if (ak_vi_get_channel_attr(handle, &cur_attr)) {
		ak_print_error_ex("ak_vi_get_channel_attr failed!\n");
	}

	return 0;
}

/* init video input */
static void *venc_init_video_in(void)
{
	/* open video input device */
	void *handle = ak_vi_open(VIDEO_DEV0);
	if (handle == NULL) {
		ak_print_error_ex("vi open failed\n");
		return NULL;
	}
	ak_print_normal("\tvi open ok\n");

	/* set capture resolution */
	if (venc_set_resolution(handle)) {
		ak_print_error_ex("\tset resolution failed\n");
		ak_vi_close(handle);
		handle = NULL;
		return NULL;
	}

	/* set capture fps */
	ak_vi_set_fps(handle, 25);
	ak_print_normal("\tcapture fps: %d\n", ak_vi_get_fps(handle));

	return handle;
}

/* open video encode handle */
static void *venc_demo_open_encoder(int index)
{
	struct encode_param param = {0};

	switch (index) {
	case 0:
		param.width = resolutions[sub_index].width;
		param.height = resolutions[sub_index].height;
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
		param.width = resolutions[main_index].width;
		param.height = resolutions[main_index].height;
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
		param.width = resolutions[sub_index].width;
		param.height = resolutions[sub_index].height;
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
		param.width = resolutions[sub_index].width;
		param.height = resolutions[sub_index].height;
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
static void venc_stop_stream(enum encode_group_type grp)
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
static void *venc_save_stream_thread(void *arg)
{
	long int tid = ak_thread_get_tid();
	ak_print_normal_ex("start a work thread, tid: %ld\n", tid);

	struct video_handle *handle = (struct video_handle *)arg;

	/* first, open file */
	FILE *fp = venc_open_file(handle->enc_type);
	ak_print_error_ex("########### open ###########\n");
	int looptimes = enc_count;

	/*
	 * according number of total save frame, iterate
	 * specific times
	 */
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
		venc_save_data(fp, stream.data, stream.len);

		/* release frame */
		ret = ak_venc_release_stream(handle->stream_handle, &stream);
		looptimes--;

		if ((looptimes % 3) == 0)
			ak_sleep_ms(10);
	}

	/* when write done, close file */
	if (fp)
		fclose(fp);

	/* stop video encode */
	venc_stop_stream(handle->enc_type);
	work_nr--;

	ak_print_normal_ex("### thread id: %ld exit ###\n", ak_thread_get_tid());
	ak_thread_exit();

	return NULL;
}

/*
 * venc_init_video_input - init video input module,
 * 							after this, you can init video encode
 * first[IN]: first path of isp.cfg
 * second[IN]: second path of isp.cfg
 * return: 0 on success, -1 failed
 * notes: if first isp.cfg match ok, second path is useless.
 */
static int venc_init_video_input(const char *first, const char *second)
{
	/* get sensor id, match config file */
	const char *path[2] = {first, second};
	int ret = 0, i = 0;
	do {
		ret = ak_vi_match_sensor(path[i]);
		i++;
	} while (ret != 0 && i < 2);

	/* first, only when match sensor ok you can do next things */
	if (ret != 0 && i == 2) {
		ak_print_error_ex("vi match config failed\n");
		return -1;
	}

	/* one video input device, only open one time vi for encode */
	video_in_handle = venc_init_video_in();
	if (!video_in_handle) {
		ak_print_error_ex("video input init faild, exit\n");
		return -1;
	}

	return 0;
}

/*
 * venc_show_channel_res - check main and sub channel resolution
 *                         config, if select is invalid, it will
 *                         set global channel index to a invalid
 *                         value to avoid crash
 * main[IN]: main channel resolution selection index
 * sub[IN]:	sub channel resolution selection index
 */
static void venc_show_channel_res(int main, int sub)
{
	/* main channel value check */
	if (main < 6 || main > 11) {
		ak_print_error_ex("main channel resolution select out of range,"
				" its value squre is [6, 11]\n");
		main_index = INVALID_INDEX;
	} else
		ak_print_notice("\tmain channel resolution: w: %u, h: %u\n",
				resolutions[main].width, resolutions[main].height);

	/* sub channel value check */
	if (sub < 0 || sub > 5) {
		ak_print_error_ex("sub channel resolution select out of range,"
				" its value squre is [0, 5]\n");
		sub_index = INVALID_INDEX;
	} else
		ak_print_notice("\tsub channel resolution: w: %u, h: %u\n",
				resolutions[sub].width, resolutions[sub].height);
}

/*
 * stream flow to get and save data
 */
static int venc_start_stream(enum encode_group_type grp)
{
	struct video_handle *handle = &ak_venc[grp];

	ak_print_normal_ex("stream encode mode, start encode group: %d\n", grp);

	/* init encode handle */
	handle->venc_handle = venc_demo_open_encoder(grp);
	if (!handle->venc_handle) {
		ak_print_error_ex("video encode open type: %d failed\n", grp);
		return -1;
	}

	/* start capture and encode */
	if(ak_vi_capture_on(video_in_handle)) {
		ak_print_error_ex("video in capture on failed\n");
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
	ak_thread_create(&handle->ak_venc_tid, venc_save_stream_thread,
			(void *)handle, 100*1024, 90);
	work_nr++;

	return 0;
}

/*
 * venc_switch_resulution - switch current resolution to a ramdom
 * 							valid resolution to test video encode
 * 							switch resolution function
 * return: 0 on success, -1 failed;
 */
static int venc_switch_resulution(void)
{
	ak_print_notice_ex("\t\t Switching resolution ...\n");

	if (main_index + 1 > 11)
		main_index -= 1;
	else
		main_index += 1;

	if (sub_index + 1 > 5)
		sub_index -= 1;
	else
		sub_index -= 1;

	venc_show_channel_res(main_index, sub_index);
	if (main_index == INVALID_INDEX || sub_index == INVALID_INDEX) {
		ak_print_error_ex("invalid argument\n");
		return -1;
	}

	/* call vi interface to set new resolution */
	struct video_channel_attr attr;
	memset(&attr, 0, sizeof(struct video_channel_attr));

	attr.res[VIDEO_CHN_MAIN].width = resolutions[main_index].width;
	attr.res[VIDEO_CHN_MAIN].height = resolutions[main_index].height;
	attr.res[VIDEO_CHN_SUB].width = resolutions[sub_index].width;
	attr.res[VIDEO_CHN_SUB].height = resolutions[sub_index].height;

	attr.res[VIDEO_CHN_MAIN].max_width = 1920;
	attr.res[VIDEO_CHN_MAIN].max_height = 1080;
	attr.res[VIDEO_CHN_SUB].max_width = 640;
	attr.res[VIDEO_CHN_SUB].max_height = 480;
	
	attr.crop.left = 0;
	attr.crop.top = 0;
	attr.crop.width = resolutions[main_index].width;
	attr.crop.height = resolutions[main_index].height;

	/* change channel resolution */
	int ret = ak_vi_change_channel_attr(video_in_handle, &attr);
	if (ret) {
		ak_print_notice_ex("change attribute failed\n");
		return -1;
	}

	return 0;
}

/*
 * venc_test_picture - save picture test function
 */
void venc_test_picture(void)
{
	int test_times = 20;

	ak_print_normal_ex("begin\n");
	while (test_times > 0) {
		struct video_input_frame f;
		struct video_stream s = {0};
		void *picture;

		/* get yuv by sigle step */
		if (!ak_vi_get_frame(video_in_handle, &f)) {
			/* open encoder */
			picture = venc_demo_open_encoder(ENCODE_PICTURE);

			/* sigle encode mode */
			ak_venc_send_frame(picture, f.vi_frame[VIDEO_CHN_SUB].data,
					f.vi_frame[VIDEO_CHN_SUB].len, &s);
			/*
			 * release yuv, notice that the time you occupy
			 * yuv should as little as possible
			 */
			ak_vi_release_frame(video_in_handle, &f);

			/* do you primary things here */
			FILE *fp = venc_open_file(ENCODE_PICTURE);
			if (fp) {
				venc_save_data(fp, s.data, s.len);
				fclose(fp);
			}

			/* after procee, close encoder */
			ak_venc_close(picture);
			picture = NULL;
			free(s.data);
			test_times--;
			ak_sleep_ms(1000);
		} else
			ak_sleep_ms(20);
	}
	ak_print_normal_ex("end\n");
}

#ifdef AK_RTOS
int cmd_venc_demo(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
	ak_print_normal("------------ video encode demo start ------------\n");

	if (argc != 4) {
		venc_usage(argv[0]);
		return -1;
	}

	venc_register_signal();

	ak_cmd_server_register(ANYKA_IPC_PORT, "venc_demo7000");

	if (access(SAVE_PATH, W_OK) != 0) {
		if (mkdir(SAVE_PATH, 0755)) {
			ak_print_error_ex("mkdir: %s, %s\n", SAVE_PATH, strerror(errno));
			return -1;
		}
	}

	enc_count = atoi(argv[1]);
	ak_print_normal("\tencode %d frames\n", enc_count);

	/* check resolutions */
	main_index = atoi(argv[2]);
	sub_index = atoi(argv[3]);

	venc_show_channel_res(main_index, sub_index);
	if (main_index == INVALID_INDEX || sub_index == INVALID_INDEX) {
		ak_print_error_ex("invalid argument\n");
		return -1;
	}

	/* match sensor, if success, open vi */
	if (venc_init_video_input(FIRST_ISP_PATH, BACK_ISP_PATH)) {
		ak_print_normal_ex("system init failed\n");
		return -1;
	}

	/* if need to know how to change video resolution, set 1 */
	int switch_res = 0;

	/* start */
	venc_start_stream(ENCODE_MAINCHN_NET);
	//venc_start_stream(ENCODE_SUBCHN_NET);

#if VENC_PICTURE_TEST
	venc_test_picture();
#endif

	/* switch resulotion */
	if (switch_res) {
		venc_switch_resulution();
		venc_start_stream(ENCODE_MAINCHN_NET);
		//venc_start_stream(ENCODE_SUBCHN_NET);
	}
	while (work_nr > 0)
		ak_sleep_ms(100);

	/* close video input device */
	ak_print_normal("closing video input ...\n");
	ak_vi_capture_off(video_in_handle);
	ak_vi_close(video_in_handle);
	video_in_handle = NULL;
	ak_sleep_ms(10);

	ak_cmd_server_unregister(ANYKA_IPC_PORT);

	ak_print_normal("video encode demo exit ...\n");

	return 0;
}

/*****************************************
 ** @brief register inidemo command
 ** @param [void]  
 ** @return 0
 ******************************************/
#ifdef AK_RTOS
static int cmd_venc_demo_reg(void)
{
    cmd_register("vencdemo", cmd_venc_demo, help);
    return 0;
}

cmd_module_init(cmd_venc_demo_reg);

#endif
