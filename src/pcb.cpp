/*
 * pcb.cpp
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#include "pcb.h"

extern Lista* allPCBs;
extern volatile PCB* running;
extern volatile unsigned lock;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread, ID id){

	this->sp = 0;
	this->ss = 0;
	this->bp = 0;
	this->stack = 0;
	this->timeSlice = timeSlice;
	this->createStack(stackSize);

	this->state = NEW;
	this->myThread = thread;
	this->threadID = id;

	this->blocked = new Lista();


	waitingTime = 0;
	timeLeft = 0;
	signalHappened = 0;

	if(allPCBs != 0)
		allPCBs->add(this);

}

PCB::~PCB(){

	if(allPCBs != 0)
		allPCBs->deletePCB(this);
	delete[] stack;
	delete blocked;

}

void PCB::createStack(StackSize stackSize){

	if(stackSize>65536){
		stackSize=65536;
	}
	int size = stackSize/sizeof(unsigned);

#ifndef BCC_BLOCK_IGNORE
	this->stack = new unsigned[size];
	stack[size-1] = 0x200;
	stack[size-2] = FP_SEG(wrapper);
	stack[size-3] = FP_OFF(wrapper);

	ss = FP_SEG(stack+size-12);
	sp = FP_OFF(stack+size-12);
	bp = sp;

#endif

}

void PCB::wrapper(){

	running->myThread->run();

	lock = 0;
	if(running->blocked != 0){
		if(running->blocked->size() != 0){
			running->blocked->release();
		}
	}
	running->state = FINISHED;
	lock = 1;
	dispatch();

}





