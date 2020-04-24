/*
 * Pressure.h
 *
 *  Created on: Mar 6, 2020
 *      Author: melinda
 */

#ifndef PRESSURE_H_
#define PRESSURE_H_

#include "I2C.h"

class Pressure {
public:
	Pressure(I2C&& i2c, uint8_t const i2c_address, uint8_t const i2c_read_command);
	virtual ~Pressure();
	int getPressure();
private:
	I2C i2c;
	uint8_t const i2c_address;
	uint8_t i2c_read_command;
	uint8_t data[3];
};

#endif /* PRESSURE_H_ */
