/*
 * SimpleMenu.h
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#ifndef SIMPLEMENU_H_
#define SIMPLEMENU_H_
#include <vector>
#include "PropertyEdit.h"
#include <atomic>
#include "Imutex.h"
#include <mutex>

extern std::atomic<bool> mode;

class SimpleMenu {
public:
	enum menuEvent {
		up,
		down,
		ok,
		back,
		show
	};
	SimpleMenu();
	virtual ~SimpleMenu();
	void addItem(PropertyEdit* item);
	void event(menuEvent e);
private:
	std::vector<PropertyEdit*> items;
	int position;
	Imutex mutex;
};

#endif /* SIMPLEMENU_H_ */
