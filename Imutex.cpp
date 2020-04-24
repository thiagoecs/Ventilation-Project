/*
 * Imutex.cpp
 *
 *  Created on: 13 Feb 2020
 *      Author: Jayne Uni
 */

#include "chip.h"
#include "Imutex.h"

std::atomic_int Imutex::counter { 0 };

Imutex::Imutex() : enable(false) {
	counter = counter + 1;
}

Imutex::~Imutex() {
	counter = counter - 1;
}

void Imutex::lock() {
	if (counter > 1)
		int test = 0;

	enable = (__get_PRIMASK() & 1) == 0;
	__disable_irq();
}

void Imutex::unlock()
{
	if(enable) __enable_irq();
}
