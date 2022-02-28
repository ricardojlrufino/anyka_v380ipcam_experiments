
// Retired from ak3918ev300v18/platform/libplat/demo/aec_ex_demo
// Adapted by: Ricardo JL Rufino

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <ak_common.h>
#include "ak_global.h"
#include "ak_ai.h"
#include "ak_ao.h"
#include "ak_thread.h"

#define AEC_EX_AI_VOLUME	5			/* aec ex demo ai volume */

/* ai data save path */
#define AEC_EX_PLAY_PATH	"/tmp/record.pcm"
char *audio_file;

#define AEC_EX_SAVE_TIME	(30*1000)	/* save ai data time, unit: second */
#define AEC_EX_SAMPLE_RATE	(8000)	    /* sample rate */


struct aec_ex_param {
	int run_flag;			/* run flag */
	void *handle;           /* ai/ao handle */
	FILE *fp;			    /* pcm file file handle */
	ak_pthread_t tid;	    /* thread tid */
};

static struct aec_ex_param pcm_in = {0};	/* ai parameter */

static int ai_save_time = AEC_EX_SAVE_TIME;
static int aec_pre_status = AK_FALSE;           /* 1-enable; 0-disable */
static int aec_cur_status = AK_FALSE;           /* 1-enable; 0-disable */
static int aec_status_switch = AK_FALSE;        /* 1-switch; 0-do not switch */

static inline void close_audio(void)
{
    if (pcm_in.run_flag) {
        ak_thread_join(pcm_in.tid);
    }

    if (pcm_in.handle) {
        ak_ai_set_aec(pcm_in.handle, AUDIO_FUNC_DISABLE);
    }

	/* ai close */
    if(pcm_in.fp) {
    	fclose(pcm_in.fp);
    	pcm_in.fp = NULL;
    }
    if (pcm_in.handle) {
        ak_ai_close(pcm_in.handle);
        pcm_in.handle = NULL;
    }
}

static int pcm_file_create(void)
{
//	char time_str[20] = {0};
//	char file_path[255] = {0};
//	struct ak_date date;
//
//	ak_get_localdate(&date);
//	ak_date_to_string(&date, time_str);
//    sprintf(file_path, "/tmp/%s.pcm", time_str);

	pcm_in.fp = fopen(audio_file, "w+");
	if(!pcm_in.fp) {
		printf("open pcm file err\n");
		return AK_FAILED;
	}

	printf("open save file: %s OK\n", audio_file);
	return AK_SUCCESS;
}

static void *read_ad_thread(void *arg)
{
	unsigned long long start_ts = 0;
	unsigned long long end_ts = 0;
	unsigned long pre_seq_no = 0;
	struct frame frame = {0};

	while (pcm_in.run_flag) {
		/* get the pcm data frame */
		if (ak_ai_get_frame(pcm_in.handle, &frame, 0) < 0) {
			ak_sleep_ms(10);
			continue;
		}

		if(fwrite(frame.data, frame.len, 1, pcm_in.fp) < 0) {
    		printf("write file err\n");
    		break;
    	}

		if (frame.seq_no != (pre_seq_no + 1)) {
			ak_print_normal_ex("audio: ts=%llu, len=%u, seq_no=%lu\n",
    			frame.ts, frame.len, frame.seq_no);
		}
		pre_seq_no = frame.seq_no;

		if(0 == start_ts) {
			start_ts = frame.ts;
			end_ts = frame.ts;
		}
		end_ts = frame.ts;
		ak_ai_release_frame(pcm_in.handle, &frame);

		if((end_ts - start_ts) >= ai_save_time) {
			break;
		}
		if (aec_cur_status != aec_pre_status) {
		    /* reset aec */
	        ak_ai_set_aec(pcm_in.handle, aec_cur_status);
	        aec_pre_status = aec_cur_status;
	        aec_status_switch = AK_TRUE;
		}
	}

	ak_print_normal("\t read_ad_pcm exit\n\n");
	ak_thread_exit();
	return NULL;
}

static void read_ad_run(void)
{
	/* set source */
	if (ak_ai_set_source(pcm_in.handle, AI_SOURCE_MIC) != 0)
		ak_print_error_ex("set ai source mic fail\n");
	else
		ak_print_normal_ex("set ai source mic success\n");

	ak_print_notice_ex("current aec enable=%d\n", aec_cur_status);
	aec_pre_status = aec_cur_status;
	/* set aec */
	ak_ai_set_aec(pcm_in.handle, aec_cur_status);

	/* enable nr&agc */
	ak_ai_set_nr_agc(pcm_in.handle, AUDIO_FUNC_ENABLE);

	/* set resample */
	ak_ai_set_resample(pcm_in.handle, AUDIO_FUNC_DISABLE);

	/* volume is from 0 to 12, volume 0 is mute */
	// ak_ai_set_volume(pcm_in.handle, AEC_EX_AI_VOLUME);
	// NOTE (RICARDO): This is not working on v380 IP CAM

	/* clear ai buffer */
	ak_ai_clear_frame_buffer(pcm_in.handle);

	/* start capture will begin to get frame */
	ak_ai_start_capture(pcm_in.handle);

	pcm_in.run_flag = AK_TRUE;

	/* capture thread begin */
	int ret = ak_thread_create(&(pcm_in.tid), read_ad_thread,
			NULL, ANYKA_THREAD_MIN_STACK_SIZE, -1);
	if (ret) {
		ak_print_error("create read_ad_thread thread FAILED, ret=%d\n", ret);
		return;
	}
}

static void process_signal(int sig)
{
	ak_print_notice("\t signal %d caught", sig);
	if((SIGTERM == sig) || (SIGINT == sig) || (SIGSEGV == sig)){
		pcm_in.run_flag = AK_FALSE;
	}
}

static int register_signal(void)
{
	signal(SIGSEGV, process_signal);
	signal(SIGINT, process_signal);
	signal(SIGTERM, process_signal);
	signal(SIGCHLD, SIG_IGN);
	return 0;
}



static int init_aec_ai(void)
{
	if(!pcm_in.fp) {
		if(pcm_file_create()) {
			return AK_FAILED;
		}
	}

	/* init ai */
	struct pcm_param param = {0};

	/* sample bits only support 16 bit */
	param.sample_bits = 16;

	/* driver set to AUDIO_CHANNEL_MONO */
	param.channel_num = AUDIO_CHANNEL_MONO;

	/* set sample rate */
	param.sample_rate = AEC_EX_SAMPLE_RATE;

	/* ai open */
    pcm_in.handle = ak_ai_open(&param);
    if(!pcm_in.handle) {
        close_audio();
    	return AK_FAILED;
    }

    /* ak_ai_set_frame_interval must use before ak_ai_get_frame */
    int ret = ak_ai_set_frame_interval(pcm_in.handle, AMR_FRAME_INTERVAL);
	if (!ret) {
		ak_print_normal_ex("frame interval=%d\n", AMR_FRAME_INTERVAL);
	}

	return ret;
}



/**
 * open ai, do not open ao, test aec enable/disable
 */
void record(){
	if (!init_aec_ai()) {
		/* ai run */
		read_ad_run();

		close_audio();
	}
}

/**
 * argv[0]: this
 * argv[1]: (optional) target record file
 */
int main(int argc, char **argv) {

	if (argc == 1) {
		ak_print_warning("Using default file: %s \n", AEC_EX_PLAY_PATH);
        audio_file = AEC_EX_PLAY_PATH;
	}else{
		audio_file = argv[1];
	}

	register_signal();

	record();

	printf("----- %s exit -----\n", argv[0]);
	return 0;

	return 0;
}


