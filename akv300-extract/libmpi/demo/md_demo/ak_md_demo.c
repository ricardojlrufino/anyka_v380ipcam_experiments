/**
* Copyright (C) 2018 Anyka(Guangzhou) Microelectronics Technology CO.,LTD.
* File Name: ak_md_demo.c
* Description: This is a simple example to show how the md module working.
* Notes:
* History: V1.0.0
*/

#include "ak_common.h"
#include "ak_vi.h"
#include "ak_md.h"

#define AREA_H_NUM 4	//block num in horizontal direction, must > 0, and can divide 32.
#define AREA_V_NUM 2	//block num in vertical direction, must > 0, and can divide 24.

/**
 * move_detect: md main function
 * @vi_handle[IN]: opened vi handle
 * return: 0 success, -1 failed
 * notes:
 */
static int move_detect(void *vi_handle)
{
	int ret = AK_FAILED;
	int md_time;
	int md_mode = 1;		//0 global mode, 1 area mode
	int detect_time = 10;   //demo run time in seconds
	int sensitivity = 50;	//1-100,  larger value means more sensitive
	struct ak_timeval start_tv;
	struct ak_timeval end_tv;

	ak_get_ostime(&start_tv);
	ak_print_normal("*** start to init md module.***\n");

	/*
	* step 1:md init
	* default parameters: detect in 10 fps
	*/
	if (ak_md_init(vi_handle) < 0) {
		ak_print_normal("md init fail\n");
		return ret;
	}

	/*
	* we have 2 modes to do detect , global mode and area mode, you can choose one
	*/
	if (0 == md_mode) {	/*md in global mode*/
		/*
		* step 2:set sensitivity
		*/
		if (AK_SUCCESS != ak_md_set_global_sensitivity(sensitivity)) {
			ak_print_normal("set_global_sensitivity fail\n");
			goto EXIT;
		}
		
		/*
		* step 3:md enable
		*/
		if (AK_SUCCESS != ak_md_enable(1)) {
			ak_print_normal("md_enable fail\n");
			goto EXIT;
		}

		/*
		* step 4:get result
		* you can create a new thread to do it 
		*/
		do {
			if (1 == ak_md_get_result(&md_time, NULL, -1))
				ak_print_normal("detected moving at time:%s\n", ak_seconds_to_string((time_t)md_time));

			ak_get_ostime(&end_tv);
			/* run time is over and exit */
			if (ak_diff_ms_time(&end_tv, &start_tv) > (detect_time * 1000))
				break;
			else
				ak_sleep_ms(100);
			
		} while(1);

	} else {	/*md in area mode*/
		int area_sens[AREA_V_NUM * AREA_H_NUM] = {0};
		char pch[AREA_V_NUM * AREA_H_NUM] = {0};
		int v, h;

		/*
		* step 2:set sensitivity
		*/
		for (v=0; v<AREA_V_NUM; v++) {
			for(h=0; h<AREA_H_NUM; h++) {
				area_sens[v * AREA_H_NUM + h] = sensitivity;
			}
		}
			
    	if (AK_SUCCESS != ak_md_set_area_sensitivity(AREA_H_NUM, AREA_V_NUM, area_sens)) {
			ak_print_normal("set_area_sensitivity fail\n");
			goto EXIT;
		}

		/*
		* step 3:md enable
		*/
		if (AK_SUCCESS != ak_md_enable(1)) {
			ak_print_normal("md_enable fail\n");
			goto EXIT;
		}

		/*
		* step 4:get result
		* you can create a new thread to do it 
		*/
		do {
			if (1 == ak_md_get_result(&md_time, pch, -1)) {
				ak_print_normal("detected moving at time:%s\n", ak_seconds_to_string((time_t)md_time));
				
				for (v=0; v<AREA_V_NUM; v++) {
					for (h=0; h<AREA_H_NUM; h++) {
						if (pch[v * AREA_H_NUM + h] > 0)
							ak_print_normal("(v %d h %d)%d\n", v, h, pch[v * AREA_H_NUM + h]);
					}
				}
			}

			ak_get_ostime(&end_tv);
			/* run time is over and exit */
			if (ak_diff_ms_time(&end_tv, &start_tv) > (detect_time * 1000))
				break;
			else
				ak_sleep_ms(100);
			
		} while(1);
	}
	
	ret = AK_SUCCESS;
	
EXIT:

	/*
	* step 5:md destroy
	*/
	ak_md_destroy();
	ak_print_normal("*** md exit ***\n");

	return ret;
}

/**
 * Preconditions:
 * your main video progress must stop
 */
int main(int argc, char **argv)
{
	ak_print_normal("*****************************************\n");
	ak_print_normal("***** md version: %s *****\n", ak_md_get_version());
    ak_print_normal("*****************************************\n");
	
	int ret = -1;								//return value
	void *vi_handle = NULL;						//vi operating handle
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
		goto exit;
	}

	/* 
	 * open video input device
	 */
	vi_handle = ak_vi_open(VIDEO_DEV0);
	if (NULL == vi_handle) {
		ak_print_normal("vi device open failed\n");	
		goto exit;
	}

	/* 
	 * set vi working parameters 
	 * default parameters: 25fps, day mode, auto frame-control
	 */
	ret = ak_vi_set_channel_attr(vi_handle, &attr);
	if (ret) {
		ak_vi_close(vi_handle);
		goto exit;
	}

	/* 
	 * start capture frames
	 */
	ret = ak_vi_capture_on(vi_handle);
	if (ret) {
		ak_vi_close(vi_handle);
		goto exit;
	}

    /*
    	* md main function 
    	*/
	move_detect(vi_handle);

	/*
	 * release resource
	 */
	ak_vi_capture_off(vi_handle);

	ret = ak_vi_close(vi_handle);

exit:
	/* exit */
	ak_print_normal("exit md demo\n");

	return ret;
}
