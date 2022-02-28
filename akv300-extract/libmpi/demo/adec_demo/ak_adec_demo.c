#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

#include "ak_common.h"
#include "ak_global.h"
#include "ak_ao.h"
#include "ak_adec.h"
#include "ak_ipc_srv.h"

#define RECORD_READ_LEN		 4096 /* read audio file buffer length */

static FILE* open_record_file(const char *record_file)
{
	FILE *fp = fopen(record_file, "r");
	if(NULL == fp) {
		ak_print_error_ex("open %s failed\n", record_file);
		return NULL;
	}

	ak_print_normal("open record file: %s OK\n", record_file);
	return fp;
}

/**
 * decode_and_play - send the stream data to decode
 * @adec_handle[IN]: adec handle
 * @data[IN]: audio file data
 * @read_len[IN]: audio file data length
 * notes:
 */
static void decode_and_play(void *adec_handle, unsigned char *data, int read_len)
{
	int send_len = 0;
	int dec_len = 0;

	while (read_len > 0) {
		/* send stream to decode */
		dec_len = ak_adec_send_stream(adec_handle, &data[send_len], read_len, 0);
		if (dec_len < 0) {
			ak_print_error_ex("write pcm to DA error!\n");
			break;
		}

		read_len -= dec_len;
		send_len += dec_len;
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

	/* get time */
	ak_get_ostime(&cur_time);
	if (ak_diff_ms_time(&cur_time, &print_time) >= 1000) {
		ak_get_ostime(&print_time);
		ak_print_normal(".");
	}
}

static void read_record_file(void *adec_handle, FILE *fp, int dec_type)
{
	int read_len = 0;
	int total_len = 0;
	unsigned char data[RECORD_READ_LEN] = {0};

	/* amr file,have to skip the head 6 byte */
	if (AK_AUDIO_TYPE_AMR == dec_type) {
		read_len = fread(data, 1, 6, fp);
	}

	while(AK_TRUE){
		/* read the record file stream */
		memset(data, 0x00, sizeof(data));
		read_len = fread(data, sizeof(char), sizeof(data), fp);

		if(read_len > 0) {
			total_len += read_len;
			/* play roop */
			decode_and_play(adec_handle, data, read_len);
			print_playing_dot();
			ak_sleep_ms(10);
		} else if(0 == read_len) {
			/* read to the end of file */
			ak_print_normal("\n\tread to the end of file\n");
			break;
		} else {
			ak_print_error("\nread, %s\n", strerror(errno));
			break;
		}
	}
	/* read over the file, must call ak_adec_read_end */
	ak_adec_notice_stream_end(adec_handle);
	ak_print_notice_ex("%s exit\n", __func__);
}

static void process_signal(int sig)
{
	ak_print_notice("\t signal %d caught", sig);
	if((SIGTERM == sig) || (SIGINT == sig) || (SIGSEGV == sig)){
		exit(EXIT_FAILURE);
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

static int parse_adec_type(const char *adec_type)
{
	int ret = -1;
	char type[10] = {0};
	int type_len = strlen(adec_type);

	/* the type length <= 5,such as g711u */
	if (type_len <= 5) {
		sprintf(type, "%s", adec_type);

		int i = 0;
		for (i=0; i<type_len; ++i) {
			type[i] = tolower(type[i]);
		}

		ak_print_notice_ex("audio decode type: %s\n", type);

		/* check the audio file type */
		if (0 == strcmp(type, "mp3")) {
			ret = AK_AUDIO_TYPE_MP3;
		} else if (0 == strcmp(type, "amr")) {
			ret = AK_AUDIO_TYPE_AMR;
		} else if (0 == strcmp(type, "aac")) {
			ret = AK_AUDIO_TYPE_AAC;
		} else if (0 == strcmp(type, "g711a")) {
			ret = AK_AUDIO_TYPE_PCM_ALAW;
		} else if (0 == strcmp(type, "g711u")) {
			ret = AK_AUDIO_TYPE_PCM_ULAW;
		} else if (0 == strcmp(type, "pcm")) {
			ret = AK_AUDIO_TYPE_PCM;
		} else if (0 == strcmp(type, "dra")) {
			/* ret = AK_AUDIO_TYPE_DRA; */
			ret = AK_AUDIO_TYPE_UNKNOWN;
		} else {
			/* unknow type */
			ret = AK_AUDIO_TYPE_UNKNOWN;
		}
	}
	return ret;
}

/**
 * argv[0]: ak_adec_demo
 * argv[1]: input audio sample rate
 * argv[2]: audio decode type,[mp3/amr/aac/g711a/g711u/pcm]
 * argv[3]: appointed audio file, absolute path
 * note: read the appointed audio file, decode, and then output to DA
 */
int main(int argc, char **argv)
{
	if (argc != 5) {
		printf("usage: %s [sample rate] [channel num] [type] [audio file path]\n",
				argv[0]);
		printf("eg.: %s 8000 2 mp3 /mnt/20161123-153020.mp3\n", argv[0]);
		printf("support type: [mp3/amr/aac/g711a/g711u/pcm]\n");
		return AK_FAILED;
	}

	ak_print_normal("----- %s -----\n", argv[0]);
	ak_print_normal("version: %s\n\n", ak_adec_get_version());
	register_signal();

	/* get audio file type */
	int type = parse_adec_type(argv[3]);
	if (type < 0) {
		ak_print_error_ex("unsupport audio decode type %s\n", argv[2]);
		return AK_FAILED;
	}

	/* get channel number */
	int channel_num = atoi(argv[2]);
	if (channel_num < 1 || channel_num > 2) {
		ak_print_error_ex("channel_num value in [1~2]\n");
		return AK_FAILED;
	}

	/* open audio output */
	struct pcm_param param = {0};
	/* sample bits only support 16 bit */
	param.sample_bits = 16;
	param.channel_num = channel_num;

	/* get sample rete */
	param.sample_rate = atoi(argv[1]);

	/* AMR only support sample rete=8000,channel num=1 */
	if (AK_AUDIO_TYPE_AMR == type) {
		if (param.sample_rate != 8000 || channel_num != 1) {
			printf("note: amr only support 8000 sample rate and channel 1 !\n");
			return AK_FAILED;
		}
	}

	/* open audio file */
	FILE *fp = open_record_file(argv[4]);
	if(NULL == fp) {
		return AK_FAILED;
	}

	ak_cmd_server_register(ANYKA_IPC_PORT, "adec_demo7000");

	/* ao open */
    void *ao_handle = ak_ao_open(&param);
    if(NULL == ao_handle) {
    	ak_print_normal("\t ak_ao_open failed !\n");
    	goto fp_end;
    }

    /* ak_ao_open OK, then do same setting */

	/* enable speaker must set before ak_ao_send_frame */
    ak_ao_enable_speaker(ao_handle, AUDIO_FUNC_ENABLE);

	/* dac volume is from 0 to 6,volume 0 is mute */
	ak_ao_set_dac_volume(ao_handle, 6);

	/* aslc volume is from 0 to 6, 0 is not use aslc */
	ak_ao_set_aslc_volume(ao_handle, 2);

	/* ak_ao_set_resample have to use before ak_ao_send_frame */
	ak_ao_set_resample(ao_handle, AUDIO_FUNC_DISABLE);

	/* before ak_ao_send_frame,must clear frame buffer */
	ak_ao_clear_frame_buffer(ao_handle);

	/* open audio encode */
	struct audio_param adec_param = {0};
	adec_param.type = type;

	/* sample bits only support 16 bit */
	adec_param.sample_bits = 16;

	/* driver set to AUDIO_CHANNEL_MONO */
	adec_param.channel_num = channel_num;
	adec_param.sample_rate = param.sample_rate;

	/* open adec */
    void *adec_handle = ak_adec_open(&adec_param);
	if(NULL == adec_handle) {
		ak_print_normal("\t ak_adec_open failed! \n");
    	goto ao_end;
    }

	/* bind audio output & decode, request stream */
	void *stream_handle = ak_adec_request_stream(ao_handle, adec_handle);
	if(NULL == stream_handle) {
		ak_print_normal("\t ak_adec_request_stream failed! \n");
    	goto adec_end;
    }

	/* read audio file stream and write to DA, use decode handle send stream */
    read_record_file(adec_handle, fp, type);

	/* decode end, cancel stream */
    ak_adec_cancel_stream(stream_handle);
    stream_handle = NULL;

adec_end:
	/* adec close */
	if(NULL != adec_handle) {
    	ak_adec_close(adec_handle);
    	adec_handle = NULL;
    }

ao_end:
    if(NULL != ao_handle) {
		/* ao disable speaker */
    	ak_ao_enable_speaker(ao_handle, AUDIO_FUNC_DISABLE);
    	/* ao close */
    	ak_ao_close(ao_handle);
    	ao_handle = NULL;
    }

fp_end:
    if(NULL != fp) {
    	fclose(fp);
    	fp = NULL;
    }
    ak_print_normal("----- %s exit -----\n", argv[0]);

	ak_cmd_server_unregister(ANYKA_IPC_PORT);

	return 0;
}
