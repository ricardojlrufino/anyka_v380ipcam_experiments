/*
 * include/plat-anyka/adkey.h
 */

#ifndef __ADKEY_H
#define __ADKEY_H

#include <mach/gpio.h>
#include "notify.h"

typedef enum {
	PLUGIN_NODEV = 0x0,
	PLUGIN_DEV1 = 0x1,							//dev1
	PLUGIN_DEV2 = 0x2,							//dev2
	PLUGIN_DEV3 = 0x4,							//dev3   
	PLUGIN_DEV4 = 0x8,							//dev4
	PLUGIN_DEV12 = PLUGIN_DEV1|PLUGIN_DEV2,		//dev12
	PLUGIN_DEV13 = PLUGIN_DEV1|PLUGIN_DEV3,		//dev13
	PLUGIN_DEV23 = PLUGIN_DEV2|PLUGIN_DEV3,		//dev23
	PLUGIN_DEV123 = PLUGIN_DEV1|PLUGIN_DEV2|PLUGIN_DEV3,	//dev123
	
	PLUGIN_INVALID,
} T_ad_check;


#define PLUGIN_MMC					PLUGIN_DEV1			//sd card
#define PLUGIN_SDIO					PLUGIN_DEV2			//sdio dev
#define PLUGIN_AC					PLUGIN_DEV3			//ac
#define PLUGIN_MMC_SDIO				PLUGIN_DEV12		//mmc+ac
#define PLUGIN_MMC_AC				PLUGIN_DEV13		//hp+mmc
#define PLUGIN_SDIO_AC				PLUGIN_DEV23		//sdio+ac
#define PLUGIN_MMC_SDIO_AC			PLUGIN_DEV123		//mmc+sdio+ac

#define ADDETECT_MMC_PLUGIN			ADDEDECT_DEV1_PLUGIN
#define ADDETECT_MMC_PLUGOUT		ADDEDECT_DEV1_PLUGOUT
#define ADDETECT_SDIO_PLUGIN		ADDEDECT_DEV2_PLUGIN
#define ADDETECT_SDIO_PLUGOUT		ADDEDECT_DEV2_PLUGOUT
#define ADDETECT_AC_PLUGIN			ADDEDECT_DEV3_PLUGIN
#define ADDETECT_AC_PLUGOUT			ADDEDECT_DEV3_PLUGOUT

struct adgpio_key {
	int code;			/* Key code */
	int min;			/* min voltage */
	int max;			/* max voltage */
};

struct multi_addetect {
	int unpress_min;
	int unpress_max;
	struct adgpio_key *fixkeys;
	T_ad_check plugdev;
};

struct analog_gpio_key {
	char *desc;				/* key description */
	int interval;			/* the value is poll adkey */
	int debounce_interval;	/* press key delay */
	
	struct multi_addetect *addet;
	int naddet;
	int nkey;				/* key number */

	int chanel;				/* AD channel */
	int wakeup;				/* enable ad-key wakeup from standby */
};

#if defined(CONFIG_ARCH_AK39)
static inline void adkey_wakeup_enable(int en)
{
}
#else
static inline void adkey_wakeup_enable(int en)
{
}
#endif

#endif	/* end __ADKEY_H */

