/*
 * SimpleMenu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "SimpleMenu.h"

SimpleMenu::SimpleMenu() {
	position = 0;
}

SimpleMenu::~SimpleMenu() {
	// TODO Auto-generated destructor stub
}

void SimpleMenu::addItem(PropertyEdit* item) {
	items.push_back(item);
}

void SimpleMenu::event(SimpleMenu::menuEvent e) {
	const std::lock_guard<Imutex> lock(mutex);

	if(items.size() <= 0) return;

	switch(e) {
	case ok:
		if(items[mode]->getFocus()){
			items[mode]->accept();
		}
		break;

	case back:
		items[mode]->display();
		break;

	case show:
		break;

	case up:
		if (items[mode]->getFocus())
			items[mode]->increment();
		break;

	case down:
		if (items[mode]->getFocus())
			items[mode]->decrement();
		break;
	}

	if (mainMenu)
		items[mode]->display();
	else {
		items[mode]->displayMenu();

	}
}
