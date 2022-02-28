#ifndef __AK_SENSOR_I2C_H__
#define __AK_SENSOR_I2C_H__

#include <linux/i2c.h>
#include <plat-anyka/ak_sensor.h>

s32 ak_sensor_i2c_set_client(struct i2c_client *client);

s32 sensor_read_register(T_SENSOR_I2C_DATA_S *pI2cData);

s32 sensor_write_register(T_SENSOR_I2C_DATA_S *pI2cData);

#endif
