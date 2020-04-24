/*
 * Imutex.h
 *
 *  Created on: 13 Feb 2020
 *      Author: Jayne Uni
 */

#ifndef IMUTEX_H_
#define IMUTEX_H_

#include <atomic>

class Imutex {
public:
	Imutex();
	~Imutex();
	void lock();
	void unlock();
private:
	static std::atomic_int counter;
	bool enable;
};
#endif /* IMUTEX_H_ */
