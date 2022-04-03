#ifndef LOG_H
#define LOG_H

#define ENABLE_DEBUG_TIMMING 0

#define LOG_LEVEL_ERROR     "ERROR "
#define LOG_LEVEL_WARNING   "WARN  "
#define LOG_LEVEL_INFO      "INFO  "
#define LOG_LEVEL_VERBOSE   "TRACE "
#define LOG_LEVEL_DEBUG     "DEBUG "

#ifndef LOG_TAG
#define LOG_TAG "capture"
#endif


#define APPLOG(level, color, fmt, arg...)  \
    printf(color "[%s] %s <%s:%u>: "fmt" \033[0m\n", level, LOG_TAG, __FUNCTION__, __LINE__, ##arg)

#define loge(fmt, arg...) APPLOG(LOG_LEVEL_ERROR  , "\033[40;31m", fmt, ##arg)
#define logw(fmt, arg...) APPLOG(LOG_LEVEL_WARNING, "\033[40;33m", fmt, ##arg)
#define logi(fmt, arg...) APPLOG(LOG_LEVEL_INFO   , "\033[40;36m", fmt, ##arg)
#define logv(fmt, arg...) APPLOG(LOG_LEVEL_VERBOSE, "\033[40;37m", fmt, ##arg)


#if(ENABLE_DEBUG_TIMMING)
	struct timeval start, end;
	int64_t interval_s;
	#define __LOG_TIME_START() do{gettimeofday( &start, NULL );}while(0)
	#define __LOG_TIME_END(name)   do{gettimeofday( &end, NULL ); \
								interval_s  =(int64_t)(end.tv_sec - start.tv_sec)*1000000ll; \
								printf("%s use time: %lld ms\n", name, (interval_s + end.tv_usec - start.tv_usec) / 1000);\
								}while(0)
#else

	#define __LOG_TIME_START()
	#define __LOG_TIME_END(name)

#endif


#endif
