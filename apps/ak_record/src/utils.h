#include <stdio.h>
#include <inttypes.h>
#include <time.h>

int64_t millis()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}
