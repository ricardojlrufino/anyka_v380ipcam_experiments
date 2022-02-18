#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "akgpios_copy.h"

#define AK_WAKEUP_ENABLE		1
#define AK_WAKEUP_DISABLE		0
#define AK_FALLING_TRIGGERED	1
#define AK_RISING_TRIGGERED		0

#define	AK_GPIO_DIR_OUTPUT		1
#define	AK_GPIO_DIR_INPUT		0
#define	AK_GPIO_INT_DISABLE		0
#define	AK_GPIO_INT_ENABLE		1
#define	AK_GPIO_INT_LOWLEVEL	0
#define	AK_GPIO_INT_HIGHLEVEL	1

#define	AK_GPIO_OUT_LOW			0
#define	AK_GPIO_OUT_HIGH		1

#define	AK_PULLUP_DISABLE		0
#define	AK_PULLUP_ENABLE		1
#define	AK_PULLDOWN_DISABLE		0
#define	AK_PULLDOWN_ENABLE		1

#define	GPIO_PIN_MODE_GPIO		0
#define	GPIO_PIN_MODE_INT		1

struct gpio_info {
    int pin;
    char pulldown;
    char pullup;
    char value;
    char dir;
    char int_pol;
};


int GPIO_FD = -1;


void pinMode(int8_t pin, int8_t mode){
	struct gpio_info gpio;

	memset(&gpio,0, sizeof(gpio));
	gpio.pulldown = -1;
	gpio.pullup = -1;
	gpio.dir = mode;
	gpio.int_pol = AK_GPIO_INT_DISABLE;

	gpio.pin = pin;
	gpio.value = AK_GPIO_OUT_LOW;

	if(GPIO_FD <= 0){
		GPIO_FD = open("/dev/akgpio", O_RDWR);
	    if(GPIO_FD < 0){
			perror("open akgpio");
			return;
		}
	}

	int ret = ioctl(GPIO_FD, SET_GPIO_FUNC, &gpio);

	if(ret >= 0) printf("gpio %d SET_GPIO_FUNC %d returns %d\n",gpio.pin, gpio.value, ret);
}

void digitalWrite(int8_t pin, int8_t val){
	struct gpio_info gpio;

	memset(&gpio,0, sizeof(gpio));
	gpio.pin = pin;
	gpio.value = val;

	int ret = ioctl(GPIO_FD, SET_GPIO_LEVEL, &gpio);

	if(ret >= 0) printf("gpio %d SET_GPIO_LEVEL %d returns %d\n",gpio.pin, gpio.value, ret);
}
