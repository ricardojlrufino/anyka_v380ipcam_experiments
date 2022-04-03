#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/omapfb.h>

// color = AARRGGBB
void drawColor(int width, int height, int color)
{
	uint32_t *canvas = new uint32_t[width * height];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(x % 2 == 0)
				canvas[x + y * width] = color;
			else
				canvas[x + y * width] = 0;
		}
	}

	int fd;
	fd = open("/dev/fb0", O_WRONLY);
	write(fd, canvas, width * height * 4);
	close(fd);
	delete[] canvas;
}

// color = AARRGGBB
void drawColor2(int width, int height, int color)
{
	int fd;
	fd = open("/dev/fb0", O_RDWR);
	uint32_t *frameBuffer = (uint32_t *)mmap(NULL, width * height * 4, PROT_WRITE, MAP_SHARED, fd, 0);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			frameBuffer[x + y * width] = color;
		}
	}

	msync(frameBuffer, width * height * 4, 0);
	munmap(frameBuffer, width * height * 4);
	close(fd);
}
