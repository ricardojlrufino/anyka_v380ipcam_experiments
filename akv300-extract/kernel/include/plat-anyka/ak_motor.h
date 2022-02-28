#ifndef __AK_MOTOR_H__
#define __AK_MOTOR_H__

#ifdef __KERNEL__
enum ak_motor_phase {
	AK_MOTOR_PHASE_A = 0,
	AK_MOTOR_PHASE_B,
	AK_MOTOR_PHASE_C,
	AK_MOTOR_PHASE_D,
	AK_MOTOR_PHASE_NUM,
};

enum ak_motor_hit {
	AK_MOTOR_HIT_LEFT = 0,
	AK_MOTOR_HIT_RIGHT,
	AK_MOTOR_HIT_NUM,
};


struct ak_motor_plat_data 
{
    struct gpio_info gpio_phase[AK_MOTOR_PHASE_NUM];
	struct gpio_info gpio_hit[AK_MOTOR_HIT_NUM];
	unsigned int irq_hit_type[AK_MOTOR_HIT_NUM];
	void (* gpio_init) (const struct gpio_info *);

	unsigned int angular_speed;
};
#endif

#define AK_MOTOR_IOC_MAGIC 		'm'
#define AK_MOTOR_SET_ANG_SPEED 		_IOW(AK_MOTOR_IOC_MAGIC, 11, int)
#define AK_MOTOR_GET_ANG_SPEED 		_IOR(AK_MOTOR_IOC_MAGIC, 12, int)
#define AK_MOTOR_TURN_CLKWISE 		_IOW(AK_MOTOR_IOC_MAGIC, 13, int)
#define AK_MOTOR_TURN_ANTICLKWISE 	_IOW(AK_MOTOR_IOC_MAGIC, 14, int)
#define AK_MOTOR_GET_HIT_STATUS 	_IOW(AK_MOTOR_IOC_MAGIC, 15, int)
#define AK_MOTOR_TURN_STOP 			_IOW(AK_MOTOR_IOC_MAGIC, 16, int)

#define AK_MOTOR_EVENT_HIT 		(1)
#define AK_MOTOR_EVENT_UNHIT 	(1<<1)
#define AK_MOTOR_EVENT_STOP 	(1<<2)

#define AK_MOTOR_HITTING_LEFT 	(1<<0)
#define AK_MOTOR_HITTING_RIGHT 	(1<<1)

#define AK_MOTOR_MIN_SPEED 		(1)
#define AK_MOTOR_MAX_SPEED 		(200)
struct notify_data
{
	int hit_num;
	int event;
	int remain_steps;
};


/// Anycloud V500 Porting.
/// /* 1. argument for start up */
/*
 * MOTOR_PARM:			set paramters
 */
#define MOTOR_PARM			_IOW(AK_MOTOR_IOC_MAGIC, 0x00, int)

/* 2. argument for real-time */
/*
 * MOTOR_SPEED_STEP:	set speed in step(hz)
 * MOTOR_SPEED_ANGLE:	set speed in angle
 * */
#define MOTOR_SPEED_STEP	_IOW(AK_MOTOR_IOC_MAGIC, 0x20, int)
#define MOTOR_SPEED_ANGLE	_IOW(AK_MOTOR_IOC_MAGIC, 0x21, int)

/* 3. base command */
/*
 * MOTOR_MOVE_LIMIT:	move clkwise or anticlkwise
 *						clkwise: val > 0, anticlkwise: val < 0
 * MOTOR_MOVE_NOLIMIT:	move clkwise or anticlkwise
 *						clkwise: val > 0, anticlkwise: val < 0
 * MOTOR_STOP:			set stop and wait for be stop finish
 * MOTOR_GET_STATUS:	get status
 * */
#define MOTOR_MOVE_LIMIT	_IOW(AK_MOTOR_IOC_MAGIC, 0x40, int)
#define MOTOR_MOVE_NOLIMIT	_IOW(AK_MOTOR_IOC_MAGIC, 0x41, int)
#define MOTOR_STOP			_IOW(AK_MOTOR_IOC_MAGIC, 0x42, int)
#define MOTOR_GET_STATUS	_IOW(AK_MOTOR_IOC_MAGIC, 0x43, int)

/* 4. extern command */
/*
 * MOTOR_RESET:			first go to ACTIVAL boundary then go to middle
 * MOTOR_MIDDLE:		turn to middle directly
 * MOTOR_CRUISE:		turn to the clkwise an anticlkwise until set stop
 * MOTOR_BOUNDARY:		move to ACTIVAL boundary
 * 						in clkwise or anticlkwise until be stop
 * */
#define MOTOR_RESET			_IOW(AK_MOTOR_IOC_MAGIC, 0x60, int)
#define MOTOR_MIDDLE		_IOW(AK_MOTOR_IOC_MAGIC, 0x61, int)
#define MOTOR_CRUISE		_IOW(AK_MOTOR_IOC_MAGIC, 0x62, int)
#define MOTOR_BOUNDARY		_IOW(AK_MOTOR_IOC_MAGIC, 0x63, int)

/*
 * motor_status:
 * @MOTOR_IS_STOP:		motor is stoped now
 * @MOTOR_IS_RUNNING:	motor is running now
 * */
enum motor_status {
	MOTOR_IS_STOP = 0,
	MOTOR_IS_RUNNING,
};

/*
 * struct motor_parm - motor parameters
 * @pos:				the current position
 * @speed_step:			speed of step(hz)
 * @steps_one_circel:	steps one circel
 * @total_steps:		steps motor can run
 * @boundary_steps:		reserved boundary steps
 */
struct motor_parm {
	int pos;
	int speed_step;
	int steps_one_circle;
	int total_steps;
	int boundary_steps;
};


/*
 * motor_message - message
 * @status:				motor working status
 * @pos:				the current position
 * @speed_step:			speed of step(hz)
 * @speed_angle:		speed of angle
 * @steps_one_circel:	steps one circel
 * @total_steps:		steps motor can run
 * @boundary_steps:		reserved boundary steps
 * @attach_timer:		attach to hardware timer
 */
struct motor_message {
	enum motor_status status;
	int pos;
	int speed_step;
	int speed_angle;
	int steps_one_circle;
	int total_steps;
	int boundary_steps;
	int attach_timer;
};



#endif
