/*
 * DigitalIoPin.h
 *
 *  Created on: 26 Feb 2020
 *      Author: Jayne Uni
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_
#include "board.h"
#include <atomic>

class DigitalIoPin {
public:
	DigitalIoPin(int port=0, int pin=0, bool input = true, bool pullup = true, bool invert = false);
	virtual ~DigitalIoPin() {};
	bool read(){ return (bool) LPC_GPIO->B[port][pin];}
	void write(bool value) {
		if(invert)
			value=!value;
		LPC_GPIO->B[port][pin]=value;
	}
private:
	// add these as needed
	int port, pin;
	bool input, pullup, invert;
};

#endif /* DIGITALIOPIN_H_ */
