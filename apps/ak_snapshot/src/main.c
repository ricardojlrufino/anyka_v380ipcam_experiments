/*
 * Copyright (c) 2021 Ricardo JL Rufino.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
* Copyright (C) Ricardo JL Rufino
* Description: Capture camera for Anyka
* Created on: 17 de fev. de 2022
*/
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ak_common.h"
#include "ak_vi.h"

// App includes
#include "convert.h"
#include "log.h"
#include "app_settings.h"
#include "utils.h"
#include "http_server.h"
#include "snapshot.h"

int res_w = 640, res_h = 480;
int enable_sound;
volatile sig_atomic_t stop;

// Debug errors
char *out_dir = SNAPSHOT_DEFAULT_DIR;
int qr_error_count = 0;
int rq_snapshot_count = 0;

// Camera vars
void *vi_handle = NULL;						//vi operating handle
struct video_channel_attr attr;				//vi channel attribute
struct video_resolution res;				//max sensor resolution

struct quirc *qr;
uint8_t * qr_buf;

struct snapshot_t snapshot_ref = {
	.count = 0,
	.ready = 0,
	.capture = 0
};

void stop_capture();


void my_handler(int s){
   printf("Caught signal %d\n",s);
   stop=1;
   usleep(200);
   exit(1);
}

void change_led(uint status){

	if(status == 0){

		// TODO: IMPLEMENT....

	}else{

	}

}

void stop_capture(){

	logi("stop_capture");

	/*
	 * step 7: release resource
	 */
	ak_vi_capture_off(vi_handle);

	ak_vi_close(vi_handle);

}

int capture_init(){

	// Creating the named file(FIFO)
	// This is used for IPC communication.
	logi("Create fifo " QRCODE_WRITE_FIFO);
	mkfifo(QRCODE_WRITE_FIFO, 0666);

	logi("image module init");

	int ret = -1;								//return value

	attr.crop.left = 0;
	attr.crop.top  = 0;
	attr.crop.width = 1280;
	attr.crop.height = 720;

	printf("VIDEO_CHN_SUB index %d \n", VIDEO_CHN_SUB);
	printf("VIDEO_CHN_MAIN index %d \n", VIDEO_CHN_MAIN);

	attr.res[1].width = 640;
	attr.res[1].height = 480;
	attr.res[1].max_width = 640;
	attr.res[1].max_height = 480;

	attr.res[0].width = 640;
	attr.res[0].height = 480;
	attr.res[0].max_width = 640;
	attr.res[0].max_height = 480;

	/*
		attr.res[1].width = 1280;
	attr.res[1].height = 720;
	attr.res[1].max_width = 1280;
	attr.res[1].max_height = 960;

	attr.res[0].width = 640;
	attr.res[0].height = 480;
	attr.res[0].max_width = 640;
	attr.res[0].max_height = 480;
*/

	/*
	 * step 1: match sensor
	 * the location of isp config can either a file or a directory
	 */
	ret = ak_vi_match_sensor(cfg);
	if (ret) {
		loge("match sensor failed\n");
		return -1;
	}

	/*
	 * step 2: open video input device
	 */
	vi_handle = ak_vi_open(VIDEO_DEV0);
	if (NULL == vi_handle) {
		loge("vi device open failed\n");
		return -1;
	}

	/*
	 * step 3: get sensor support max resolution
	 */
	ret = ak_vi_get_sensor_resolution(vi_handle, &res);
	logv("ak_vi_get_sensor_resolution\n");
	if (ret) {
		ak_vi_close(vi_handle);
		return -1;
	} else {
		attr.crop.width = res.width;
		attr.crop.height = res.height;
	}

	/*
	 * step 4: set vi working parameters
	 * default parameters: 25fps, day mode, auto frame-control
	 */
	ret = ak_vi_set_channel_attr(vi_handle, &attr);
	logv("ak_vi_set_channel_attr\n");
	if (ret) {
		loge("ak_vi_set_channel_attr FAILLL GO TO CLOSE\n");
		ak_vi_close(vi_handle);
		return -1;
	}

	/*
	 * step 5: start capture frames
	 */
	ret = ak_vi_capture_on(vi_handle);
	logv("ak_vi_capture_on\n");
	if (ret) {
		ak_vi_close(vi_handle);
		return -1;
	}

	// qrcode_init();

	return 1;
}


void debug_capture(unsigned char *rawimg, unsigned int len){

	#if DEBUG_SNAPSHOT


//	char bmp_data_path[50];
//	sprintf(bmp_data_path, "%s/preview.bmp", out_dir);
//
//		// if need capture a series of images.
//		#if DEBUG_SNAPSHOT_MULTIPLE
//		sprintf(bmp_data_path, "%s/img-%d.bmp", out_dir, rq_snapshot_count++);
//		#endif
//
//	logw(" >>>>> Saving file: %s", bmp_data_path);
//	YUVToBMP(bmp_data_path, rawimg, YUV420ToRGB24, res_w, res_h);


	if(snapshot_ref.capture == 1){
		char bmp_data_path[50];
		sprintf(bmp_data_path, "%s/preview.bmp", "/tmp");
		logw(" >>>>> Saving snapshot: %s", bmp_data_path);
		YUVToBMP(bmp_data_path, rawimg, YUV420ToRGB24, res_w, res_h);

		usleep(250); // wait to write
		snapshot_ref.count++;
		snapshot_ref.capture = 0;
		snapshot_ref.ready = 1;
	}

	#endif

}

void capture_loop(){

	struct video_input_frame frame = {{{0}, {0}}};

	logv("capture start");

	/*
	 * To get frame by loop
	 */
	while (!stop) {

		__LOG_TIME_START();

		memset(&frame, 0x00, sizeof(frame));

		/* to get frame */
		int ret = ak_vi_get_frame(vi_handle, &frame);
		if (!ret) {

			//ak_print_normal("[%d] main chn yuv len: %u\n", count,frame.vi_frame[VIDEO_CHN_MAIN].len);
			//ak_print_normal("[%d] sub  chn yuv len: %u\n\n", count,frame.vi_frame[VIDEO_CHN_SUB].len);

			unsigned int len = frame.vi_frame[VIDEO_CHN_MAIN].len;
			unsigned char *buf = frame.vi_frame[VIDEO_CHN_MAIN].data;

			debug_capture(buf, len);

			// qrcode_decode(buf, len);

			// release frame data
			ak_vi_release_frame(vi_handle, &frame);

		} else {
	        // not ready， sleep to release CPU
			ak_sleep_ms(10);
		}

		__LOG_TIME_END("capture");

		ak_sleep_ms(25); // Free CPU to do other things
						 // 25ms -> 60~70% CPU
	}

	stop_capture();
}


int parse_args(int argc, char* argv[]){

//    if(argc == 1) {
//        loge("Usage: ./camerademo -d /tmp/sd/record -w 640 -h 480");
//        return -1;
//    }

    for (;;) {
        int opt = getopt(argc, argv, ":d:w:h:s");
        if (opt == -1)
            break;
        switch (opt) {
        case '?':
            fprintf(stderr, "%s: Unexpected option: %c\n", argv[0], optopt);
            return -1;
        case ':':
            fprintf(stderr, "%s: Missing value for: %c\n", argv[0], optopt);
            return -1;
        case 'd':
            fprintf(stdout, "Using directory: %s\n", optarg);
            out_dir = optarg;
            break;
        case 's':
            enable_sound = 1;
            break;
        case 'w':
            res_w = atoi(optarg);
            break;
        case 'h':
            res_h = atoi(optarg);
            break;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {

	if (parse_args(argc, argv) < 0) return -1;

	signal(SIGINT, my_handler);

	logi("capture_init...");
	int status = capture_init();

	start_server(3000, SNAPSHOT_DEFAULT_DIR, &snapshot_ref);

	if(status){
		logi("loop...");
		capture_loop();
	}

	return 0;
}
