/*
 * thread.cpp
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "schedule.h"

class Idle;

extern volatile Thread* idleThread;
extern int contextSwitch;
extern volatile unsigned lock;
extern volatile PCB* running;
extern Lista* allPCBs;

ID Thread::idThread = 0;

Thread::Thread(StackSize stackSize, Time timeSlice){

	lock = 0;
	myID = idThread++;
	myPCB = new PCB(stackSize, timeSlice, this, myID);
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

void Thread::start(){

	lock = 0;
	if(myPCB != 0){
		if((myPCB->state == PCB::NEW) && (this->myID != idleThread->myID)){
			myPCB->state = PCB::READY;
			Scheduler::put(myPCB);
		}
	}
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

void Thread::waitToComplete(){

	lock = 0;
	if((this->myID ==  running->threadID) || (this->myID == idleThread->myID)){ //da li je pozvala za samu sebe i da li je pozvala za idle nit
		lock = 1;
		if(contextSwitch){
			dispatch();
		}
		return;
	}else{
		if(myPCB != 0){
			if(this->myPCB->state != PCB::FINISHED){
					running->state = PCB::BLOCKED;
					this->myPCB->blocked->add((PCB*) running);
					lock = 1;
					dispatch();
			}else{
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

}

ID Thread::getId(){

	return this->myID;

}

ID Thread::getRunningId(){

	return running->threadID;

}

Thread* Thread::getThreadById(ID id){

	if(allPCBs != 0)
		return allPCBs->getPCBbyID(id);
	else
		return 0;

}

Thread::~Thread(){

	waitToComplete();
	lock = 0;
	delete myPCB;
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

PCB* Thread::getPCB(){

	return myPCB;

}



