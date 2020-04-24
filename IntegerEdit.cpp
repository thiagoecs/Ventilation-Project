/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>


IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	if(edit < upper) {
		edit++;
	}
}

void IntegerEdit::decrement() {
	if(edit > lower){
		edit--;
	}
}

void IntegerEdit::accept() {
	save();
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
	return;
}

bool IntegerEdit::getFocus() {
	//return this->focus;
	return true;
}

void IntegerEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("Mode: ");
	lcd->setCursor(0,1);
	char s[17];
	snprintf(s, 17, "   %s     ", status[mode]);
	lcd->print(s);
}

void IntegerEdit::displayMenu() {
	lcd->clear();
	lcd->setCursor(0,0);
	char s[17];
	if(mode) {
		snprintf(s, 17, "Set fan: %3d %% ", edit);
		lcd->print(s);
	} else {
		snprintf(s, 17, "Set pres: %3d Pa", edit);
		lcd->print(s);
	}

	lcd->setCursor(0, 1);
	snprintf(s, 17, "Cur pres: %3d Pa", pressure);
	lcd->print(s);
	if ((millis() - timeout) > 10000 && !mode){
		lcd->clear();
		lcd->setCursor(0, 0);
		lcd->print("Error: Pressure");
		lcd->setCursor(0, 1);
		lcd->print("Not Reachable");
	}
}




void IntegerEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}


int IntegerEdit::getValue() {
	return value;
}

void IntegerEdit::setValue(int value) {
	this->value = edit = value;
}
