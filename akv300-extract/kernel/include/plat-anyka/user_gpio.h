#ifndef __EXPORT_GPIO__
#define __EXPORT_GPIO__

#include <mach/gpio.h>

struct ak_user_gpio_pdata { 
	struct user_gpio_info *user_gpios;
	int nr_user_gpios;
};

#endif
