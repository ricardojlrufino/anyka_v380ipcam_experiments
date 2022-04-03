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

void getFrameBufferSize(int* width, int* height, int* colorWidth)
{
	int fd;
	int n;
	char str[64];

	/* Get Bit Per Pixel (usually 32-bit(4Byte) */
	fd = open("/sys/class/graphics/fb0/bits_per_pixel", O_RDONLY);
	n = read(fd, str, sizeof(str));
	str[n] = '\0';
	*colorWidth = atoi(str);
	close(fd);

	/* Get Frame Buffer Size */
	fd = open("/sys/class/graphics/fb0/virtual_size", O_RDONLY);
	n = read(fd, str, sizeof(str));
	str[n] = '\0';
	/* divide "800,600" into "800" and "600" */
	*width  = atoi(strtok(str, ","));
	*height = atoi(strtok(NULL, ","));
	close(fd);
}

void getFrameBufferSize2(int* width, int* height, int* colorWidth)
{
	struct fb_var_screeninfo var;
	int fd;
	fd = open("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &var);
	*colorWidth = var.bits_per_pixel;
	*width      = var.xres_virtual;
	*height     = var.yres_virtual;
	close(fd);
}
