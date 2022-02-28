
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

#include "gpio.h"


struct aec_ex_param {
	int run_flag;			/* run flag */
	void *handle;           /* ai/ao handle */
	FILE *fp;			    /* pcm file file handle */
	ak_pthread_t tid;	    /* thread tid */
};

static struct aec_ex_param pcm_out = {0};	/* ao parameter */

#define PCM_READ_LEN		4096		/* ao play buffer length */
#define AEC_EX_SAMPLE_RATE	(8000)	    /* sample rate */

#define AEC_EX_PLAY_PATH	"/tmp/record.pcm"

char *audio_file;

static void process_signal(int sig)
{
	ak_print_notice("\t signal %d caught", sig);
	if((SIGTERM == sig) || (SIGINT == sig) || (SIGSEGV == sig)){
		pcm_out.run_flag = AK_TRUE;
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


static inline void close_audio(void)
{
    if (pcm_out.run_flag) {
        ak_thread_join(pcm_out.tid);
    }
//    if (pcm_in.run_flag) {
//        ak_thread_join(pcm_in.tid);
//    }
//
//    if (pcm_in.handle) {
//        ak_ai_set_aec(pcm_in.handle, AUDIO_FUNC_DISABLE);
//    }

    /* ao close */
    if(pcm_out.fp) {
    	fclose(pcm_out.fp);
    	pcm_out.fp = NULL;
    }
    if (pcm_out.handle) {
        ak_ao_close(pcm_out.handle);
        pcm_out.handle = NULL;
    }

	/* ai close */
//    if(pcm_in.fp) {
//    	fclose(pcm_in.fp);
//    	pcm_in.fp = NULL;
//    }
//    if (pcm_in.handle) {
//        ak_ai_close(pcm_in.handle);
//        pcm_in.handle = NULL;
//    }
}

static FILE* open_ao_file(const char *pcm_file)
{
	FILE *fp = fopen(pcm_file, "r");
	if(NULL == fp) {
		printf("open pcm file err\n");
		return NULL;
	}

	printf("open pcm file: %s OK\n", pcm_file);
	return fp;
}


/**
 * copy_for_dual_channel - copy the same data for another channel
 * @src[IN]: audio pcm data
 * @len[IN]: audio pcm data len
 * @dest[IN]: audio pcm data new buffer
 * notes: we just copy the same pcm data for another channel
 */
static void copy_for_dual_channel(const unsigned char *src, int len,
							unsigned char *dest)
{
	int j = 0;
	int count = (len / 2);

	for (j=0; j<count; ++j) {
		dest[j * 4] = src[j * 2];
		dest[j * 4 + 1] = src[j * 2 + 1];
		dest[j * 4 + 2] = src[j * 2];
		dest[j * 4 + 3] = src[j * 2 + 1];
	}
}

/**
 * print_playing_dot - print . when playing every second
 * notes:
 */
static void print_playing_dot(void)
{
	static unsigned char first_flag = AK_TRUE;
	static struct ak_timeval cur_time;
	static struct ak_timeval print_time;

	if (first_flag) {
		first_flag = AK_FALSE;
		ak_get_ostime(&cur_time);
		ak_get_ostime(&print_time);
		ak_print_normal("\n.");
	}

	ak_get_ostime(&cur_time);
	if (ak_diff_ms_time(&cur_time, &print_time) >= 1000) {
		ak_get_ostime(&print_time);
		ak_print_normal(".");
	}
}

static int init_aec_ao(void)
{
	if (!pcm_out.fp) {
		pcm_out.fp = open_ao_file(audio_file);
	}
	if(!pcm_out.fp) {
		printf("open %s failed!\n", audio_file);
		return AK_FAILED;
	}

    int ret = AK_FAILED;
	struct pcm_param ao_param = {0};

	/* sample bits only support 16 bit */
	ao_param.sample_bits = 16;

	/* driver set to AUDIO_CHANNEL_MONO */
	ao_param.channel_num = AUDIO_CHANNEL_MONO;

	/* set sample rate */
	ao_param.sample_rate = AEC_EX_SAMPLE_RATE;

	/* ao open */
    pcm_out.handle = ak_ao_open(&ao_param);
    if(pcm_out.handle) {
    	ret = AK_SUCCESS;
    } else {
        close_audio();
    }

    return ret;
}


static void wait_play_finished(void *ao_handle)
{
    enum ao_play_status pre_status= AO_PLAY_STATUS_RESERVED;
	enum ao_play_status cur_status = AO_PLAY_STATUS_RESERVED;
    int total_time = 0;

    do {
    	/* get ao status */
    	cur_status = ak_ao_get_play_status(ao_handle);
    	if (pre_status != cur_status) {
    		pre_status = cur_status;
	    	switch(cur_status) {
	    	case AO_PLAY_STATUS_READY:
	    		ak_print_normal("play status=%d, ready to play\n", cur_status);
	    		break;
	    	case AO_PLAY_STATUS_PLAYING:
	    		ak_print_normal("play status=%d, playing, wait time: %d\n",
	    			cur_status, total_time);
	    		break;
	    	case AO_PLAY_STATUS_FINISHED:
	    		ak_print_normal("play status=%d, play finished\n", cur_status);
	    		break;
	    	default:
	    		ak_print_normal("play status=%d, reserved\n", cur_status);
	    		break;
	    	}
    	}

		/* wait the data play finish */
		if (AO_PLAY_STATUS_FINISHED != cur_status) {
			ak_sleep_ms(10);
			total_time += 10;
		}
	} while (AO_PLAY_STATUS_FINISHED != cur_status);
}


static void *write_da_thread(void *arg)
{
	int read_len = 0;
	int total_len = 0;
	unsigned char data[PCM_READ_LEN] = {0};
	unsigned char full_buf[PCM_READ_LEN * 2];

    fseek(pcm_out.fp, 0x00, SEEK_SET);
	while(pcm_out.run_flag) {
		/* read the pcm file data */
		memset(data, 0x00, sizeof(data));
		read_len = fread(data, sizeof(char), sizeof(data), pcm_out.fp);

		if(read_len > 0) {
			total_len += read_len;

			/* double channels */
			copy_for_dual_channel(data, read_len, full_buf);

			/* send frame and play */
			if (ak_ao_send_frame(pcm_out.handle, full_buf, read_len*2, 0) < 0) {
				ak_print_error_ex("write pcm to DA error!\n");
				break;
			}
			print_playing_dot();
		} else if(0 == read_len) {
		    ak_ao_notice_frame_end(pcm_out.handle);
			ak_print_normal("\n\t read to the end of file\n");
			break;
		} else {
			ak_print_error("read, %s\n", strerror(errno));
			break;
		}
		ak_sleep_ms(10);
	}

	/* wait the driver play end */
	if (0 == read_len) {
		wait_play_finished(pcm_out.handle);
	}
	ak_sleep_ms(10);

	ak_print_normal("\t write_da_pcm exit\n\n");
	ak_thread_exit();

	/* disable apeaker */
	ak_ao_enable_speaker(pcm_out.handle, AUDIO_FUNC_DISABLE);
	return NULL;
}

static void write_da_run(int run_count)
{
	/* enable speaker must set before ak_ao_send_frame */
	ak_ao_enable_speaker(pcm_out.handle, AUDIO_FUNC_ENABLE);

	/* volume is from 0 to 12, volume 0 is mute */
	// ak_ao_set_volume(pcm_out.handle, AEC_EX_AO_VOLUME);
	// NOTE (RICARDO): This is not working on v380 IP CAM

	/* ak_ao_set_resample have to use before ak_ao_send_frame */
	ak_ao_set_resample(pcm_out.handle, AUDIO_FUNC_DISABLE);

    if (0 == run_count) {
        /* before ak_ao_send_frame,must clear frame buffer */
        ak_ao_clear_frame_buffer(pcm_out.handle);
    }

    pcm_out.run_flag = AK_TRUE;
	/* ao play thread begin */
	int ret = ak_thread_create(&(pcm_out.tid), write_da_thread,
			NULL, ANYKA_THREAD_MIN_STACK_SIZE, -1);
	if (ret) {
		ak_print_error("create write_da_thread thread FAILED, ret=%d\n", ret);
		return;
	}
}



/**
 * open_ai_first - open ai first then open ao
 */
static void play(void)
{

	/* init ao */
	if (init_aec_ao()) {
		ak_print_error_ex("init aec ao failed\n");
		goto ai_first_end;
	}

	/* ao run */
    write_da_run(0);

ai_first_end:
    close_audio();
}

int main(int argc, char **argv) {

	if (argc == 1) {
		ak_print_warning("Using default file: %s \n", AEC_EX_PLAY_PATH);
        audio_file = AEC_EX_PLAY_PATH;
	}else{
		audio_file = argv[1];
	}

	// Activate speaker
	set_gpio(32, 1);

	register_signal();

	play();

	return 0;
}
