/*
 * Fan.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: thiag
 */

#include "Fan.h"
#include <cstring>
#include <cstdio>

#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "LpcUart.h"
#include "board.h"
#include "chip.h"


Fan::Fan() : node{ 2 } {
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);

	ControlWord = 0x0406; // prepare for starting

	Sleep(1000); // give converter some time to set up

	ControlWord = 0x047F; // set drive to start mode

	Sleep(1000);
}

Fan::~Fan() {
	// TODO Auto-generated destructor stub
}

bool Fan::setFrequency(int freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 50;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	freq *= 200;
	if (freq > 20000)
		freq = 20000;
	else if (freq < 0)
		freq = 0;

	Frequency = freq; // set motor frequency

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

uint16_t Fan::getFrequency() {
	ModbusRegister OutputFrequency{ &node, 102 };
	auto result = static_cast<uint16_t>(OutputFrequency);
	return result / 200;
}
