#ifndef __LINUX_I2C_TSC2007_H
#define __LINUX_I2C_TSC2007_H

enum {
	ORIGIN_TOPLEFT = 1,
	ORIGIN_BOTTOMLEFT,
	ORIGIN_TOPRIGHT,
	ORIGIN_BOTTOMRIGHT,
};

struct cp2007_intpin_info {
	unsigned int pin;
	char pulldown;  //pulldown function flag
	char pullup;    //pullup function flag
	char dir;       //direction  input/output
	char int_pol;   //interrupt polarity
	unsigned int rst_pin;
	int rst_time;
	char rst_dir;
	char rst_pol;
	char rst_not;
};


struct tscp2007_platform_data {
	u16	model;				/* 2007. */
	u16	x_plate_ohms;	/* must be non-zero value */
	u16	max_rt; /* max. resistance above which samples are ignored */
	unsigned long poll_delay; /* delay (in ms) after pen-down event
				     before polling starts */
	unsigned long poll_period; /* time (in ms) between samples */
	int	fuzzx; /* fuzz factor for X, Y and pressure axes */
	int	fuzzy;
	int	fuzzz;

	struct cp2007_intpin_info intpin_info;

	int (*get_pendown_state)(unsigned int pin);
	void (*clear_penirq)(void);		/* If needed, clear 2nd level  interrupt source */
	int (*init_platform_hw)(const struct cp2007_intpin_info *intpin_info);
	void (*exit_platform_hw)(void);

	void (*reset_platform_hw)(const struct cp2007_intpin_info *intpin_info);
};

#endif	/* __LINUX_I2C_TSC2007_H */