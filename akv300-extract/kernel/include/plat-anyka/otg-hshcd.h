/*
 * AKOTG HS register declarations and HCD data structures
 */

#ifndef __ANYKA_OTG_HS_H_
#define __ANYKA_OTG_HS_H_


#include <linux/delay.h>
#include <asm/gpio.h>

#define AKOTG_HC_HCD

#define USB_OP_MOD_REG			(AK_VA_SYSCTRL + 0x58)
#define USB_MODULE_RESET_REG	(AK_VA_SYSCTRL + 0x20)


#define USB_HC_BASE_ADDR	(AK_VA_USB)
#define H_MAXPACKET			512	   /* bytes in fifo */

struct akotghc_usb_platform_data {
	void (* gpio_init)(const struct gpio_info *info);
	struct gpio_info gpio_pwr_on;
	struct gpio_info gpio_pwr_off;
	struct gpio_info switch_onboard;
	struct gpio_info switch_extport;
};

#endif /* __ANYKA_OTG_HS_H_ */
