/*
 * DigitalIoPin.cpp
 *
 *  Created on: 26 Feb 2020
 *      Author: Jayne Uni
 */

#include "DigitalIoPin.h"
#include "board.h"
#include <atomic>


DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert) :
port{port}, pin{pin}, input{input}, pullup{pullup}, invert{invert} {

	LPC_IOCON->PIO[port][pin] = (1U + pullup) << 3 | 1U << 7 | invert << 6;

	if(input){
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	} else {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
}
