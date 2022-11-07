/*
 * idle.cpp
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#include "idle.h"
//#include "pcb.h"
extern int flagIdle;

void Idle::run(){

	while(flagIdle);

}

Idle::Idle(StackSize stackSize, Time timeSlice): Thread(stackSize, timeSlice) {}


