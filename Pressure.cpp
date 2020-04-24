/*
 * Presure.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: melinda
 */

#include "Pressure.h"

Pressure::Pressure(I2C&& i2c, uint8_t const i2c_address, uint8_t const i2c_read_command) : i2c{ i2c }, i2c_address{ i2c_address }, i2c_read_command{ i2c_read_command } {


}

Pressure::~Pressure() {
	// TODO Auto-generated destructor stub
}
int Pressure::getPressure() {
	i2c.transaction(i2c_address, &i2c_read_command, 1, data, 3);
	return static_cast<int16_t>(data[0] << 8 | data[1]) / 240 * 0.95;
}
