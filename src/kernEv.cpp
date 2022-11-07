/*
 * kernEv.cpp
 *
 *  Created on: Sep 9, 2021
 *      Author: OS1
 */

#include "kernEv.h"

class IVTEntry;

extern volatile PCB* running;
extern volatile unsigned lock;
extern int contextSwitch;

KernelEv::KernelEv(IVTNo ivtNo, Event* event){

	entry = ivtNo;
	myEvent = event;
	IVTEntry::ivts[entry]->setMyEvent(this);
	value = 0;
	isBlocked = 0;
	myPCB = (PCB*) running;

}

void KernelEv::signal(){

	lock = 0;
	if(isBlocked == 0){
		value = 1;
	}else{
		myPCB->state = PCB::READY;
		Scheduler::put(myPCB);
		isBlocked = 0;
	}
	lock = 1;
	dispatch();

}

void KernelEv::wait(){

	lock = 0;
	if(running->threadID == myPCB->threadID){
		if(value == 0){

			isBlocked = 1; //blokira se, jer se nije desio prekid

			running->state = PCB::BLOCKED;

			lock = 1;
			dispatch();

		}else{
			value = 0;
			lock = 1;
			if(contextSwitch){
				dispatch();
			}
		}
	}else{
		lock = 1;
		if(contextSwitch){
			dispatch();
		}
	}

}

