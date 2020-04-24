/*
 * IntegerEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef INTEGEREDIT_H_
#define INTEGEREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

const char status[2][7] {"Auto", "Manual"};

class IntegerEdit: public PropertyEdit {
public:
	IntegerEdit(LiquidCrystal* const lcd, std::string const title, int const lower, int const upper) : lcd(lcd), title(title), value{ lower }, edit{ lower },
	lower(lower), upper(upper), pressure{ 0 } {}
	virtual ~IntegerEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	void displayMenu();
	int getValue();
	void setValue(int value);
	void setPressure(int pressure) {
		this->pressure = pressure;
	}

private:
	void save();
	void displayEditValue();
	LiquidCrystal* const lcd;
	std::string const title;
	int value;
	int edit;
	int const lower;
	int const upper;
	int pressure;
};

#endif /* INTEGEREDIT_H_ */
