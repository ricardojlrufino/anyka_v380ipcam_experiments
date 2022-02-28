/*
 * include/plat-anyka/wifi.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __WIFI_H__
#define __WIFI_H__

#include <mach/gpio.h>

struct akwifi_platform_data {
	struct gpio_info gpio_cs;	
	struct gpio_info gpio_on;
	struct gpio_info gpio_off;
	int power_on_delay;
	int power_off_delay;
	int total_usb_ep_num;
	void (* gpio_init) (const struct gpio_info *);
};

 struct gpio_info * Akwifi_gpio_cs(void);

#endif /* __WIFI_H__ */

