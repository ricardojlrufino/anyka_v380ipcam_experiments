#ifndef __AK_BAT_H_
#define __AK_BAT_H_ __FILE__

#include <mach/gpio.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <plat/gpio_keys.h>

#define poweroff_enable 	1
#define poweroff_disable 	0

#define BAT_CHARGE_ADC_DETECT	0
#define BAT_CHARGE_GPIO_DETECT	1

struct bat_gpio{
	int is_detect_mode;				/* ac detect mode: ADC or GPIO */
	int active;						/* active value */
	int irq;						/* use for ac in or out irq */
	int delay;						/* delay irq handler */
	struct gpio_info pindata;		/* battery gpios info */

};

struct bat_info{
	int charge_full_pin;	/* use for juge if battery charge full */ 									  
	int power_on_voltage;	/* if (voltage<=power_on_voltage), disable power on */
	int power_on_correct;	/* correct power on voltage */
	int charge_min_voltage;	/* charge minute voltage */
	int max_voltage;		/* max battery voltage  */
    int min_voltage;		/* min battery voltage  */
    int power_off;			/* enable or disable machine power off in battery driver */
    int full_capacity;
    int voltage_sample;
    int poweroff_cap;
    int low_cap;
    int recover_cap;
	int cpower_on_voltage;	/* if (voltage<=cpower_on_voltage in charge), disable power on */
	unsigned int full_delay;/* time to up to full capacity, unit: minute */
	int full_voltage;		/* if voltage > full_voltage in full capacity, when in discharge,display full */
};

struct read_voltage_sample{
	int 	index;			// voltage[6] index
	int 	max;			// sample max voltage
	int 	min;			// sample min voltage
	int		sum;			// sum of read voltage
	int		sample;			/* read how many voltge sample */	
	int 	design_max;
};


struct bat_ad4{
	int up_resistance;				/* read ad4 voltage resistance */
	int dw_resistance;
	int voltage_correct;			/* correct voltage from adc1-4*/
	int adc_avdd;					/* adc avdd */
};

struct ak_bat_mach_info {

	void (* gpio_init) (const struct gpio_info *);
	struct bat_gpio usb_gpio;
	struct bat_gpio ac_gpio;
	struct bat_gpio full_gpio;
	struct bat_info bat_mach_info;
	struct bat_ad4  bat_adc;
};


int ak_bat_read_voltage(struct ak_bat_mach_info *info);

#ifdef CONFIG_STARTUP_CHECK_VOLTAGE
void ak_mach_check_bat_vol(struct ak_bat_mach_info *batinfo,
	struct ak_gpio_keys_button *pwr_gpio, int len);
#else
static inline void ak_mach_check_bat_vol(struct ak_bat_mach_info *batinfo,
	struct ak_gpio_keys_button *pwr_gpio, int len)
{
}
#endif


#endif				/* __AK_BAT_H_ */


