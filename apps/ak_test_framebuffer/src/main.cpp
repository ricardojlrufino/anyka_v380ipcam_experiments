#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "getSizeFrameBuffer.h"
#include "writeFrameBuffer.h"

// g++ main.cpp getSizeFrameBuffer.cpp readFrameBuffer.cpp writeFrameBuffer.cpp

int main()
{
	int colorWidth;
	int width;
	int height;

	/* get frame buffer size */
	getFrameBufferSize(&width, &height, &colorWidth);
	printf("%d(W) x %d(H) x %d(Bit)\n", width, height, colorWidth);

//	/* screen capture */
//	uint32_t *buffer = new uint32_t[width * height];
//	readBuffer(width, height, buffer);
//	saveFileBinary("capture.raw", buffer, width * height * 4);
//	delete[] buffer;

	/* Fill solid color */
	drawColor(width, height, 0xFFFF0000);	// RED
}


void saveFileBinary(const char* filename, void* data, int size)
{
	FILE *fp;
	fp = fopen(filename, "wb");
	fwrite(data, 1, size, fp);
	fclose(fp);
}
