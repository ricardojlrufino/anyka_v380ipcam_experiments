#ifndef __AKAPPLY_GPIO_H
#define __AKAPPLY_GPIO_H


#define APPLY_IOC_MAGIC	'f'

/*
	Command definitions which are supported by this dirver
*/
#define GET_GPIO_NUM		_IOR(APPLY_IOC_MAGIC, 40, __u8)
#define GET_AVAILABLE_GPIO	_IOR(APPLY_IOC_MAGIC, 41, __u8)
#define SET_GPIO_FUNC		_IOW(APPLY_IOC_MAGIC, 42, __u8)
#define GET_GPIO_VALUE		_IOWR(APPLY_IOC_MAGIC, 43, __u8)
#define SET_GPIO_IRQ		_IOW(APPLY_IOC_MAGIC, 44, __u8)
#define LISTEN_GPIO_IRQ		_IOW(APPLY_IOC_MAGIC, 45, __u8)
#define DELETE_GPIO_IRQ		_IOW(APPLY_IOC_MAGIC, 46, __u8)
#define SET_GPIO_LEVEL		_IOW(APPLY_IOC_MAGIC, 47, __u8)
#define SET_GROUP_GPIO_LEVEL	_IOW(APPLY_IOC_MAGIC, 48, __u8)


#ifdef __KERNEL__

struct custom_gpio_data {
	unsigned int *gpiopin;
	unsigned int	ngpiopin;
};

struct gpio_ginfo {
	int pin;
	int value;
};

struct gpio_group_info {
	struct gpio_ginfo *gpio;
	int gpio_num;
};

#endif	/* end __KERNEL__ */

#endif	/* end AKAPPLY_GPIO_H */
