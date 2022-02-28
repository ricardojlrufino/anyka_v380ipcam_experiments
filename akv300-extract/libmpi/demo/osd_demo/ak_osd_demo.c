#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ak_common.h"
#include "ak_vi.h"
#include "ak_osd.h"
#include "ak_vpss.h"
#include "ak_ipc_srv.h"

#define SAVE_FILE_DIR "/mnt/"

#define CH0_WIDTH 1280
#define CH0_HEIGHT 720
#define CH1_WIDTH 640
#define CH1_HEIGHT 360

/* default osd config info */
#define DEFAULT_FONT_BIN "/usr/local/ak_font_16.bin"
#define FONT_WIDTH	16
#define FONT_HEIGHT	16

#define DEFAULT_ISP_CONFIG "/etc/jffs2/"

#define OSD_STRING_FORMAT "CHN%dOSD%d"

char cfg_dir[100] = {0};
char font_file[100] = {0};

static void * vi_handle = NULL;

static void usage(const char *name)
{
	ak_print_normal("Usage: %s [options]\n\n"
		"options:\n"
		"-c sensor cfg file path\n"
		"-f font file path\n"
		"-h   help\n",name);
}

static int osd_init_video_input(void * handle)
{
	struct video_resolution video_res;
	struct video_channel_attr ch_attr ;

	/* get sensor resolution */
	memset(&video_res, 0, sizeof(struct video_resolution));
	if (ak_vi_get_sensor_resolution(handle, &video_res))
		ak_print_error_ex("ak_mpi_vi_get_sensor_res fail!\n");
	else
		ak_print_normal_ex("ak_mpi_vi_get_sensor_res ok! w:%d, h:%d\n",
			video_res.width, video_res.height);

	/* set video resolution and crop information */
	memset(&ch_attr, 0, sizeof(struct video_channel_attr));
	ch_attr.res[VIDEO_CHN_MAIN].width = video_res.width;
	ch_attr.res[VIDEO_CHN_MAIN].height = video_res.height;
	ch_attr.res[VIDEO_CHN_SUB].width = CH1_WIDTH;
	ch_attr.res[VIDEO_CHN_SUB].height = CH1_HEIGHT;

	ch_attr.res[VIDEO_CHN_MAIN].max_width = video_res.width;
	ch_attr.res[VIDEO_CHN_MAIN].max_height = video_res.height;
	ch_attr.res[VIDEO_CHN_SUB].max_width = CH1_WIDTH;
	ch_attr.res[VIDEO_CHN_SUB].max_height = CH1_HEIGHT;
	
	ch_attr.crop.width = video_res.width;
	ch_attr.crop.height = video_res.height;

	if (ak_vi_set_channel_attr(handle, &ch_attr)) {
		ak_print_error_ex("ak_vi_set_channel_attr fail!\n");
	} else {
		ak_print_normal_ex("ak_vi_set_channel_attr ok!\n");
	}

	memset(&ch_attr, 0, sizeof(struct video_channel_attr));
	if (ak_vi_get_channel_attr(handle, &ch_attr)) {
		ak_print_error_ex("ak_vi_get_channel_attr fail!\n");
	}

	ak_vi_set_fps(handle, 25);

	/* get current sensor fps and start capture on */
	int fps = 0;
	fps = ak_vi_get_fps(handle);
	ak_print_normal_ex("fps = %d\n",fps);

	ak_vi_capture_on(handle);

	return 0;
}

/* get YUV data from vi */
static int get_yuv_file(void * handle, int pos_id)
{
	struct video_input_frame frame = {{{0}, {0}}};
	FILE *fd = NULL;
	char name[32] = {0};
	time_t cur_secs;
	int i = 0;

	memset(&frame, 0, sizeof(struct video_input_frame));
	/*clear frame buf which is old data*/
	while(1){
		if((ak_vi_get_frame(handle, &frame) == 0)){
			ak_print_normal_ex("ts: %llu\n", frame.vi_frame[0].ts);
			ak_vi_release_frame(handle, &frame);
		}
		memset(&frame, 0, sizeof(struct video_input_frame));
		if (i++ > 3)
			break;
	}

	for (i = 0; i < 2; ) {
		memset(&frame, 0, sizeof(struct video_input_frame));
		if (ak_vi_get_frame(handle, &frame)) {
			ak_print_error_ex("ak_video_input_get_frame fail!\n");
			ak_sleep_ms(50);
			continue;
		} else {
			i++;

			ak_print_normal_ex("ak_video_input_get_frame OK! ts: %llu\n",
				frame.vi_frame[0].ts);

			/* save the YUV frame data */
			time(&cur_secs);
			for(int channel = 0; channel < VPSS_OSD_CHANNELS_MAX; channel++){
				memset(name, 0, 32);
				snprintf(name, 31, "%sCH%d_%ld%s",SAVE_FILE_DIR, channel, cur_secs, ".yuv");

				fd = fopen(name,"w+b");
				if (fd != NULL) {
					if (frame.vi_frame[channel].len == fwrite(
								frame.vi_frame[channel].data, 1,
								frame.vi_frame[channel].len, fd)) {
						ak_print_normal_ex(" save YUV file %s ok!!\n", name);
					}
					else {
						ak_print_error_ex(" save YUV file %s fail!!\n", name);
					}
					fclose(fd);
				} else {
					ak_print_error_ex("open YUV file fail!!\n");
				}
			}

			/* the frame has used,release the frame data */
			ak_vi_release_frame(handle, &frame);
			ak_sleep_ms(1000);
		}
	}

	return 0;
}

static int test_osd(void)
{
	if (ak_osd_set_font_file(FONT_WIDTH, font_file) < 0)	{
		ak_print_error_ex("ak_osd_set_font_file fail! %s\n",font_file);
		return -1;
	}
	ak_print_normal_ex("ak_osd_set_font_file ok!\n");

	if (ak_osd_init(vi_handle) < 0) {
		ak_print_error_ex("ak_osd_init fail!\n");
		return -1;
	}
	ak_print_normal_ex("ak_osd_init ok!\n");

#if 0
	/*set osd buff*/
	int channel;
	for (channel = 0; channel < 2; channel++) {

		int width = (channel == 0) ? CH0_WIDTH : CH1_WIDTH;
		int height = (channel == 0) ?  CH0_HEIGHT: CH1_HEIGHT;
		int max_w, max_h;

		if (ak_osd_get_max_rect(channel, &max_w, &max_h) < 0) {
			ak_print_error_ex("chn:%d ak_osd_get_max_rect fail!\n", channel);
			return -1;
		}
		ak_print_normal_ex("chn:%d ak_osd_get_max_rect ok, max_w: %d max_h: %d\n",
				channel, max_w, max_h);

		width = (width > max_w) ? max_w : width;
		height = (height > max_h) ? max_h : height;

		if (ak_osd_set_rect(vi_handle, channel, 0, 0, 0, width, height) < 0) {
			ak_print_error_ex("chn: %d ak_osd_set_rect fail!\n", channel);
			return -1;
		}
		ak_print_normal_ex("chn: %d ak_osd_set_rect ok!\n", channel);
	}

	/* display osd on 4 position: "leftup","leftdown","rightup","rightdown" */
	int osd_pos_id;
	for (osd_pos_id = 0; osd_pos_id < 4; osd_pos_id++) {
		/* draw string osd on channel 0 */
		if (ak_osd_draw_str(0, 0, startx[osd_pos_id] * 2 ,
					starty[osd_pos_id] * 2, osd_str1, 12) < 0) {
			ak_print_error_ex("ak_osd_draw_str fail!\n");
			return -1;
		}
		ak_print_normal_ex("ak_osd_draw_str ok!\n");

		/* draw string osd on channel 1 */
		if (ak_osd_draw_str(1, 0, startx[osd_pos_id], starty[osd_pos_id],
					osd_str2, 12) < 0) {
			ak_print_error_ex("ak_osd_draw_str fail!\n");
			return -1;
		}
		ak_print_normal_ex("ak_osd_draw_str ok!\n");
	}

	/* draw matrix to osd */
	{
		int size = 48;
		int matrix_size = 96 * 48 / 2;
		unsigned char *osd_char = calloc(1, matrix_size);
		memset(osd_char, 0x12, matrix_size);
		if(ak_osd_draw_matrix(0, 0, size * 2, 144, size, size * 2, osd_char, matrix_size) < 0 )
			ak_print_error_ex("ak_osd_draw_matrix fail!\n");
		else
			ak_print_normal_ex("ak_osd_draw_matrix ok!\n");
		if(ak_osd_draw_matrix(0, 0, size * 6, 144, size * 2, size, osd_char, matrix_size) < 0 )
			ak_print_error_ex("ak_osd_draw_matrix fail!\n");
		else
			ak_print_normal_ex("ak_osd_draw_matrix ok!\n");
		free(osd_char);
	}
#else
	int video_chn;
	int osd_chn;
	char osd_string[80];
	unsigned short us_osd_string[80];

	for (video_chn = 0; video_chn < VPSS_OSD_CHANNELS_MAX; video_chn++)
	for (osd_chn = 0; osd_chn < VPSS_OSD_NUM_PER_VIDEO_CHN; osd_chn++) {
		int width;
		int height;
		int max_w, max_h;

		if (ak_osd_get_max_rect(video_chn, &max_w, &max_h) < 0) {
			ak_print_error_ex("chn:%d ak_osd_get_max_rect fail!\n", video_chn);
			return -1;
		}

		sprintf(osd_string, OSD_STRING_FORMAT, video_chn, osd_chn);

		width = strlen(osd_string) * FONT_WIDTH;
		height = FONT_HEIGHT * 2;

		width = (width > max_w) ? max_w : width;
		height = (height > max_h) ? max_h : height;

		int x = FONT_WIDTH * 4;
		int y = FONT_HEIGHT + (height + FONT_HEIGHT) * osd_chn;

		if (ak_osd_set_rect(vi_handle, video_chn, osd_chn, x, y, width, height) < 0) {
			ak_print_error_ex("video_chn:%d osd_chn:%d ak_osd_set_rect fail!\n", video_chn, osd_chn);
			return -1;
		}

		for (int i = 0; i < strlen(osd_string); i++)
			us_osd_string[i] = osd_string[i];

		if (ak_osd_draw_str(video_chn, osd_chn, 0, 0,
					us_osd_string, strlen(osd_string)) < 0) {
			ak_print_error_ex("video_chn:%d osd_chn:%d ak_osd_draw_str fail!\n", video_chn, osd_chn);
			return -1;
		}
	}
#endif

	/* get screen data and save it in file. yuv format */
	if (get_yuv_file(vi_handle, 0) < 0) {
		ak_print_error_ex("get_yuv_file fail!\n");
	}

	ak_osd_destroy();

	return 0;
}

static int match_sensor(void)
{
	if (ak_vi_match_sensor(cfg_dir)) {
		ak_print_error_ex(" match sensor cfg fail!%s\n",cfg_dir);
		return AK_FAILED;
	} else {
		ak_print_normal_ex(" ak_vi_match_sensor ok!\n");
	}

	return 0;
}

int main(int argc, char **argv)
{
	ak_print_notice_ex("*** osd demo begin compile:%s %s %s ***\n",
			__DATE__, __TIME__, ak_osd_get_version());

	int ch;
	memset((void *)cfg_dir, 0 ,100);
	memset((void *)font_file, 0 ,100);
	
	/* parse input options */
	while ((ch = getopt(argc, argv, "c:f:h")) != EOF) {
		switch (ch) {
			case 'c':
				strncpy(cfg_dir ,optarg , 99);
				break;
			case 'f':
				strncpy(font_file ,optarg , 99);
				break;
			case 'h':
			case 'H':
			default:
				usage(argv[0]);
				return 0;
				break;
		}
	}

	ak_cmd_server_register(ANYKA_IPC_PORT, "osd_demo7000");
	/* set default value if do not provide */
	if(strlen(cfg_dir) == 0)
		strncpy(cfg_dir ,DEFAULT_ISP_CONFIG, 99);
	if(strlen(font_file) == 0)
		strncpy(font_file ,DEFAULT_FONT_BIN, 99);

	/* match current working sensor */
	if (match_sensor()) {
		ak_print_error_ex("match sensor fail!\n");
		return -1;
	}

	vi_handle = ak_vi_open(VIDEO_DEV0);
	if (NULL == vi_handle) {
		ak_print_error_ex("ak_vi_open fail!\n");
		return -1;
	} else {
		ak_print_normal_ex("ak_vi_open ok!\n");
	}

	if(osd_init_video_input(vi_handle) == 0){
		test_osd();
	}
	
	ak_vi_capture_off(vi_handle);
	ak_vi_close(vi_handle);
	vi_handle = NULL;

	ak_cmd_server_unregister(ANYKA_IPC_PORT);
	ak_print_notice_ex("********** osd demo finish ************\n");

	return 0;
}
