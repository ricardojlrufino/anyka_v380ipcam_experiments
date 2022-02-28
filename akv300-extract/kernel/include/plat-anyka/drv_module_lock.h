#ifndef __DRV_MODULE_LOCK__
#define __DRV_MODULE_LOCK__

#include <mach/gpio.h>

/**
 * @brief driver module define.
 * define all the moudle
 */
typedef enum
{
    DRV_MODULE_AD = 0,
    DRV_MODULE_DA,
    DRV_MODULE_I2S,
    DRV_MODULE_UART,
    DRV_MODULE_CAMERA,
    DRV_MODULE_DETECT,
    DRV_MODULE_RTC,
    DRV_MODULE_KEYPAD,
    DRV_MODULE_VTIMER,
    DRV_MODULE_TOUCH_SCREEN,

    DRV_MODULE_UVC,
    DRV_MODULE_USB_DISK,
    DRV_MODULE_USB_CDC,
    DRV_MODULE_USB_CAMERA,
    DRV_MODULE_USB_ANYKA,
    DRV_MODULE_USB_CMMB,

    DRV_MODULE_UDISK_HOST,
    DRV_MODULE_UVC_HOST,

    DRV_MODULE_USB_BUS,

    DRV_MODULE_FREQ,
    DRV_MODULE_HTIMER,
    DRV_MODULE_LCD_RGB,
    DRV_MODULE_LCD_MPU,
    DRV_MODULE_NAND,
    DRV_MODULE_SDMMC,
    DRV_MODULE_SPI,
    DRV_MODULE_SDIO,
    DRV_MODULE_I2C,
    
    DRV_MODULE_GPIO_EDGE,

    DRV_MODULE_JPEG_CODEC
} E_DRV_MODULE;

typedef enum {
	AK_MODULE_LOCK_1,
	AK_MODULE_LOCK_2,
	AK_MODULE_LOCK_3,
	AK_MODULE_LOCK_4,
	AK_MODULE_LOCK_5,
	AK_MODULE_LOCK_6,
	AK_MODULE_LOCK_7,
	AK_MODULE_LOCK_8,
	AK_MODULE_LOCK_9,
	AK_MODULE_LOCK_10,
	AK_MODULE_COUNT,
} E_LOCK_NAME;

typedef enum {
	TYPE_LOCK_SEMAPHORE,
	TYPE_LOCK_MUTEX,
	TYPE_LOCK_SPINLOCK,
	TYPE_LOCK_SPINLOCK_IRQ
} E_LOCK_TYPE;

struct ak_drv_module_lock {
	E_DRV_MODULE drv_mod_name;
	E_LOCK_NAME lock_name;
	E_LOCK_TYPE	lock_type;
	unsigned long flags;
	T_GPIO_SHAREPIN_CFG sharepin_cfg;
};

struct ak_drv_sharepin_lock {
	E_DRV_MODULE drv_mod_name;
	E_LOCK_NAME lock_name;
	E_LOCK_TYPE lock_type;
	unsigned long flags;
	void (*config_share_pin)(void);
};


/**
 * function: acquire lock. protect driver module to do normally. 
 *		as GPIOs are part of drivers module.
 * param: [in]driver module name 
 * ret: void
 */
int ak_drv_module_lock(E_DRV_MODULE drv_mod_name);

/**
 * function: release lock. protect driver module to do normally. 
 * 		as GPIOs are part of drivers module.
 * param: [in]driver module name 
 * ret: void
 */
void ak_drv_module_unlock(E_DRV_MODULE drv_mod_name);

/**
 * function: acquire lock. protect driver module to do normally. 
 * 		as the GPIO is defined only at board(name is product also).
 * param: [in]driver module name 
 * ret: 
 *	0: if acquire lock successed; 
 *	<0: indicate the driver  is not need lock
 */
int ak_drv_sharepin_lock(E_DRV_MODULE drv_mod_name);

/**
 * function: release lock. protect driver module to do normally. 
 * 		as the GPIO is defined only at board(name is product also).
 * param: [in]driver module name 
 * ret: 
 *	0: if acquire lock successed; 
 *	<0: indicate the driver  is not need lock
 */
void ak_drv_sharepin_unlock(E_DRV_MODULE drv_mod_name);

/**
 * function: acquire lock. protect driver module to do normally. 
 * param: [in]driver module name 
 * ret: void
 */
void ak_drv_module_protect(E_DRV_MODULE drv_mod_name);

/**
 * function: release lock. protect driver module to do normally. 
 * param: [in]driver module name 
 * ret: void
 */
void ak_drv_module_unprotect(E_DRV_MODULE drv_mod_name);

void ak_set_sharepin_lock_table(
	struct ak_drv_sharepin_lock *shpin_lock_table, int count, void **lock_array);


#endif	/* __DRV_MODULE_LOCK__ */

