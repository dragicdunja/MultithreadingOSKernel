/*
 * kernEv.h
 *
 *  Created on: Sep 9, 2021
 *      Author: OS1
 */

#ifndef KERNEV_H_
#define KERNEV_H_

#include "event.h"
#include "pcb.h"

class KernelEv {
public:

	KernelEv(IVTNo ivtNo, Event* event);

	void signal();
	void wait();

	int value;
	IVTNo entry;
	Event* myEvent;

	PCB* myPCB;
	int isBlocked;

};



#endif /* KERNEV_H_ */
