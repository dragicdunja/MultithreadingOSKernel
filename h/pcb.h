/*
 * pcb.h
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "list.h"

class Lista;

class PCB{
public:

	enum ThreadState {

		NEW = 0,
		READY = 1,
		BLOCKED = 2,
		FINISHED = 3

	};

	~PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread* myThread, ID threadID);

	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	ThreadState state;
	Thread* myThread;
	ID threadID;
	Lista* blocked;

	void createStack(StackSize stackSize);

	static void wrapper();

	Time waitingTime;
	Time timeLeft;
	volatile int signalHappened;

};



#endif /* PCB_H_ */
