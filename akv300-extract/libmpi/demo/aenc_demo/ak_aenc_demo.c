#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

#include "ak_common.h"
#include "ak_global.h"
#include "ak_ai.h"
#include "ak_aenc.h"
#include "ak_ipc_srv.h"

#define AENC_ADC_MAX_VOLUME		12		/* max volume according to ao */

struct aenc_demo {
	int save_time;		/* save time */
	char *save_path;	/* save file path */
	FILE *fp;			/* save file handle */
};

struct aenc_demo aenc = {0};

/**  
 * create_record_file - create the audio record file
 * @enc_type[IN]: encode type [mp3/amr/aac/g711a/g711u/pcm]
 * notes: 
 */
static int create_record_file(int enc_type)
{
	char time_str[20] = {0};
	char file_path[255] = {0};
	struct ak_date date;

	/* get the current time */
	ak_get_localdate(&date);
	ak_date_to_string(&date, time_str);

    /* set the file full path */
	switch(enc_type) {
	case AK_AUDIO_TYPE_AAC:
		sprintf(file_path, "%s%s.aac", aenc.save_path, time_str);
		break;
	case AK_AUDIO_TYPE_AMR:
		sprintf(file_path, "%s%s.amr", aenc.save_path, time_str);
		break;
	case AK_AUDIO_TYPE_MP3:
		sprintf(file_path, "%s%s.mp3", aenc.save_path, time_str);
		break;
	case AK_AUDIO_TYPE_PCM_ALAW:
		sprintf(file_path, "%s%s.g711a", aenc.save_path, time_str);
		break;
	case AK_AUDIO_TYPE_PCM_ULAW:
		sprintf(file_path, "%s%s.g711u", aenc.save_path, time_str);
		break;
	case AK_AUDIO_TYPE_PCM:
		sprintf(file_path, "%s%s.pcm", aenc.save_path, time_str);
		break;
	default:
		sprintf(file_path, "%s%s", aenc.save_path, time_str);
		break;
	}

	aenc.fp = fopen(file_path, "w+");
	if(NULL == aenc.fp) {
		ak_print_error_ex("create amr file err\n");
		return AK_FAILED;
	}
	
	/* write AMR file head */
	if (AK_AUDIO_TYPE_AMR == enc_type) {
		const unsigned char amrheader[]= "#!AMR\n";
		fwrite(amrheader, sizeof(amrheader) - 1, 1, aenc.fp);
	}
	ak_print_normal_ex("create amr file: %s OK\n", file_path);	
	return AK_SUCCESS;
}

/**  
 * write_stream_to_file - write stream to audio record file
 * @stream_head[IN]: stream list head
 * notes: 
 */
static void write_stream_to_file(struct list_head *stream_head)
{
	struct aenc_entry *entry = NULL;
	struct aenc_entry *ptr = NULL;

	list_for_each_entry_safe(entry, ptr, stream_head, list) {
		if(entry) {
		/* write data to file */
			if(fwrite(entry->stream.data, entry->stream.len, 1, aenc.fp) < 0) {
				ak_print_error_ex("write file err\n");
			}
		}
		/* after write to file,release stream */
		ak_aenc_release_stream(entry);
	}
}

static void write_record_file(void *stream_handle)
{
	struct ak_timeval enc_time;
	struct ak_timeval cur_time;
	struct list_head stream_head;
	
	INIT_LIST_HEAD(&stream_head);
	ak_get_ostime(&enc_time);
	ak_get_ostime(&cur_time);
	while(AK_TRUE) {
		ak_get_ostime(&cur_time);
		if(ak_diff_ms_time(&cur_time, &enc_time) >= aenc.save_time) {
			/* time is up */
			ak_print_normal("write file time is up\n");
			break;
		}

		/* get audio stream list */
		if (!ak_aenc_get_stream(stream_handle, &stream_head)) {
			/* write data to file */
			write_stream_to_file(&stream_head);
		}	
        ak_sleep_ms(10);
	}
	ak_print_normal_ex("thread exit\n\n");
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

static int parse_aenc_type(const char *aenc_type)
{
	int ret = -1;
	char type[10] = {0};
	int type_len = strlen(aenc_type);
	
	/* the type length <= 5,such as g711u */
	if (type_len <= 5) {
		sprintf(type, "%s", aenc_type);

		int i = 0;
		for (i=0; i<type_len; ++i) {
			type[i] = tolower(type[i]);
		}
		ak_print_notice_ex("audio encode type: %s\n", type);
		
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
		}
	}
	return ret;	
}

static int get_ai_source(const char *source)
{
	int ret = AK_FAILED;
	char type[10] = {0};
	int type_len = strlen(source);
	
	if (type_len <= 6) {
		sprintf(type, "%s", source);

		int i = 0;
		for (i=0; i<type_len; ++i) {
			type[i] = tolower(type[i]);
		}
		ak_print_notice_ex("audio source: %s\n", type);
		
		/* check the audio source type */
		if (0 == strcmp(type, "mic")) 
			ret = AI_SOURCE_MIC;		
		else if (0 == strcmp(type, "linein"))
			ret = AI_SOURCE_LINEIN;
	}
	return ret;
}

/**  
 * set_frame_interval - set frame interval and set to ai
 * @ai_handle[IN]: ai handle
 * @enc_type[IN]: audio encode type
 * @sample_rate[IN]: sample rate
 * notes: 
 */
static void set_frame_interval(void *ai_handle, int enc_type, 
				unsigned int sample_rate)
{
	int interval = -1;
	
	switch (enc_type) {
	case AK_AUDIO_TYPE_AAC:
		interval = ((1024 *1000) / sample_rate); /* 1k data in 1 second */
		break;
	case AK_AUDIO_TYPE_AMR:
		interval = AMR_FRAME_INTERVAL;
		break;
	case AK_AUDIO_TYPE_PCM_ALAW:	/* G711, alaw */
	case AK_AUDIO_TYPE_PCM_ULAW:	/* G711, ulaw */
		interval = AUDIO_DEFAULT_INTERVAL;
		break;
	case AK_AUDIO_TYPE_PCM:
		interval = AUDIO_DEFAULT_INTERVAL;
		break;
	case AK_AUDIO_TYPE_MP3:
		if (sample_rate >= 8000 && sample_rate <= 24000) {
			interval = 576*1000/sample_rate;
		} else { // sample_rate =32000 or 44100 or 48000
			interval = 1152*1000/sample_rate;
		}
		break;
	default:	
		interval = AUDIO_DEFAULT_INTERVAL;
		break;
	}

	/* ak_ai_set_frame_interval must use before ak_ai_get_frame */
	if (AK_SUCCESS == ak_ai_set_frame_interval(ai_handle, interval)) {
		ak_print_normal("frame interval=%d\n", interval);	
	}
}

/**
 * argv[0]: ak_aenc_demo
 * argv[1]: sample rate
 * argv[2]: audio encode type
 * argv[3]: encode file save path
 * argv[4]: record time, unit: second
 * argv[5]: audio volume
 * note: We will generate a appointed audio file in the appointed file path
 * 		such as 20161123-153020.mp3
 */
int main(int argc, char **argv)
{
	if (argc != 8) {
		printf("usage: %s [sample rate] [channel] [type] [save path] [time(secs)] [volume] [mic/linein]\n",
				argv[0]);
		printf("eg.: %s 8000 1 mp3 /mnt/ 10 7 mic\n", argv[0]);
		printf("support type: [mp3/amr/aac/g711a/g711u/pcm]\n");
		printf("note: max sample rate 48000Hz\n");
		printf("note: max volume 12\n");
		return AK_FAILED;
	}

	ak_print_normal("----- %s -----\n", argv[0]);
	ak_print_normal("version: %s\n\n", ak_aenc_get_version());
	register_signal();

	int enc_type = parse_aenc_type(argv[3]);
	if (enc_type < 0) {
		ak_print_error_ex("unsupport audio encode type %s\n", argv[2]);
		return AK_FAILED;
	}

	/* get save time,change time from second to millisecond */
	aenc.save_time = atoi(argv[5]) * 1000;
	if (0 >= aenc.save_time) {
		printf("save time must more than 1 second!\n");
		return AK_FAILED;
	}

	/* get volume */
	int volume = atoi(argv[6]);
	if (volume > AENC_ADC_MAX_VOLUME || volume < 0) {
		printf("volume is 0~%d\n", AENC_ADC_MAX_VOLUME);
		return AK_FAILED;
	}

	/* get source */
	int source = get_ai_source(argv[7]);
	if (AK_FAILED == source) {
		printf("source must be mic or linein\n");
		return AK_FAILED;
	}

	/* get save psth */
	aenc.save_path = argv[4];

	if (NULL == aenc.fp) {
		if(AK_FAILED == create_record_file(enc_type)) {
			return AK_FAILED;
		}
	}

	ak_cmd_server_register(ANYKA_IPC_PORT, "aenc_demo7000");

	/* open audio input */
	struct pcm_param ai_param = {0};
	
	/* sample bits only support 16 bit */
	ai_param.sample_bits = 16;

	/* channel number */
	ai_param.channel_num = atoi(argv[2]);;

	/* get sample rete */
	ai_param.sample_rate = atoi(argv[1]);

	/* ai open */
    void *ai_handle = ak_ai_open(&ai_param);
    if (NULL == ai_handle) {
    	goto fp_end;
    }
    ak_print_normal("\t 1. ak_ai_open OK\n");

	/* set aec,aec only support 8K sample */
	ak_ai_set_aec(ai_handle, AUDIO_FUNC_DISABLE);

	/* set nr and agc,nr&agc only support 8K sample */
	ak_ai_set_nr_agc(ai_handle, AUDIO_FUNC_ENABLE);
	
	/* set resample */
	ak_ai_set_resample(ai_handle, AUDIO_FUNC_DISABLE);

	/* volume is from 0 to 12,volume 0 is mute */
	// ak_ai_set_volume(ai_handle, volume);


	if (ak_ai_set_source(ai_handle, source))
		ak_print_error_ex("set ai source %s fail\n", argv[7]);
	else
		ak_print_normal_ex("set ai source %s success\n", argv[7]);
	
	/* clear ai buffer */
	ak_ai_clear_frame_buffer(ai_handle);

	set_frame_interval(ai_handle, enc_type, ai_param.sample_rate);
	
	/* start capture will begin to get frame */
	ak_ai_start_capture(ai_handle);

	/* open audio encode */
	struct audio_param aenc_param = {0};
	aenc_param.type = enc_type;
	aenc_param.sample_bits = ai_param.sample_bits;
	aenc_param.channel_num = ai_param.channel_num;
	aenc_param.sample_rate = ai_param.sample_rate;

	/* open aenc */
    void *aenc_handle = ak_aenc_open(&aenc_param);
	if (NULL == aenc_handle) {
    	goto ai_end;
    }
    ak_print_normal("ak_aenc_open OK\n");

	if (enc_type == AK_AUDIO_TYPE_AAC) {
	    struct aenc_attr attr;
	    attr.aac_head = AENC_AAC_SAVE_FRAME_HEAD;
	    ak_aenc_set_attr(aenc_handle, &attr); 
    }

	// ak_aenc_set_frame_default_interval(aenc_handle, 40);

	/* bind audio input & encode */
	void *stream_handle = ak_aenc_request_stream(ai_handle, aenc_handle);
	if (NULL == stream_handle) {
		ak_print_error_ex("ak_aenc_request_stream failed\n");
    	goto aenc_end;
    }
    ak_print_normal("ak_aenc_request_stream OK\n");

    /* get audio stream and write it to file */
    write_record_file(stream_handle);

	if (NULL != stream_handle) {
    	ak_aenc_cancel_stream(stream_handle);
    	stream_handle = NULL;
    }

aenc_end:
	if (NULL != aenc_handle) {
		/* aenc close */
    	ak_aenc_close(aenc_handle);
    	aenc_handle = NULL;
    }

ai_end:
    if (NULL != ai_handle) {
    	ak_ai_stop_capture(ai_handle);
    	/* ai close */
    	ak_ai_close(ai_handle);
    	ai_handle = NULL;
    }

fp_end:
    if (NULL != aenc.fp) {
    	fclose(aenc.fp);
    	aenc.fp = NULL;
    }
    
	ak_cmd_server_unregister(ANYKA_IPC_PORT);
    printf("----- %s exit -----\n", argv[0]);

	return 0;
}

