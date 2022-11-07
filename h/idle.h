/*
 * idle.h
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class Idle: public Thread{
public:
	Idle(StackSize stackSize = defaultStackSize, Time timeSlice = 1);
	void run();
};

#endif /* IDLE_H_ */
