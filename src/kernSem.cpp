/*
 * kernSem.cpp
 *
 *  Created on: Aug 23, 2021
 *      Author: OS1
 */

#include "kernSem.h"
#include "listSem.h"

extern ListSem* allSems;
extern volatile PCB* running;
extern volatile unsigned lock;
extern volatile unsigned lockSem;
extern int contextSwitch;

ID KernelSem::id = 0;

int KernelSem::getValue(){

	return value;

}

KernelSem::KernelSem(Semaphore* semaphore, int init){

	myID = id++;
	mySemaphore = semaphore;
	value = init;
	blocked = new Lista();

	if(allSems != 0)
		allSems->add(this);

}

KernelSem::~KernelSem(){ //ne smemo obrisati semafor dok ima blokiranih niti na njemu, prvo odblokiramo sve iz liste

	if(value < 0 && blocked != 0){
		blocked->release();
	}
	value = 0;
	if(allSems != 0)
		allSems->deleteSem(this);
	delete blocked;

}

int KernelSem::wait(Time maxTimeToWait){

	lock = 0;
	lockSem = 0;

	value = value - 1;

	int ret = 1;

	if(value < 0){
		ret = block(maxTimeToWait);
	}

	lockSem = 1;
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

	return ret;
}

void KernelSem::signal(){
	lock = 0;
	lockSem = 0;

	value = value + 1;

	if(value <= 0){
		deblock();
	}

	lockSem = 1;
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

void KernelSem::deblock(){

	PCB* pcb = 0;
	if(blocked != 0)
		pcb = blocked->takeFirst();
	if(pcb != 0)
		pcb->signalHappened = 1;

}

int KernelSem::block(Time maxTimeToWait){

	running->waitingTime = maxTimeToWait;
	running->timeLeft = maxTimeToWait;
	running->signalHappened = 0;
	running->state = PCB::BLOCKED;

	if(blocked != 0)
		blocked->add((PCB*) running);

	lock = 1;
	lockSem = 1;
	dispatch();
	lock = 0;
	return running->signalHappened;

}




